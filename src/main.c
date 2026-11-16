#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define PORTA_A DT_NODELABEL(gpioa)
#define PINO_PEDESTRE_NS_R 4
#define PINO_PEDESTRE_NS_G 5

LOG_MODULE_REGISTER(megabrain, LOG_LEVEL_DBG);

const struct device *gpioa_dev = DEVICE_DT_GET(PORTA_A);

K_MUTEX_DEFINE(semaforo_pedestre_NS);

void semaforo_pedestres_nortesul_G(void *p1, void *p2, void *p3) {
    int ret1;
    if(!device_is_ready(gpioa_dev)) {
        LOG_ERR("ERRO: Porta A não está pronta");
        return 1;
    }
    ret1  = gpio_pin_configure(gpioa_dev, PINO_PEDESTRE_NS_G, GPIO_OUTPUT_INACTIVE);
    if(ret1 < 0) {
        LOG_ERR("ERRO: falhou em configurar pino %d", PINO_PEDESTRE_NS_G);
    }
    while(1) {
        k_mutex_lock(&semaforo_pedestre_NS, K_FOREVER);
        gpio_pin_set(gpioa_dev, PINO_PEDESTRE_NS_G, 1);
        k_msleep(4000);
        gpio_pin_set(gpioa_dev, PINO_PEDESTRE_NS_G, 0);
        k_mutex_unlock(&semaforo_pedestre_NS);
    }
}

void semaforo_pedestres_nortesul_R (void *p1, void *p2, void *p3) {
    int ret2;
    if(!device_is_ready(gpioa_dev)) {
        LOG_ERR("ERRO: Porta A não está pronta");
        return 1;
    }
    ret2  = gpio_pin_configure(gpioa_dev, PINO_PEDESTRE_NS_R, GPIO_OUTPUT_INACTIVE);
    if(ret2 < 0) {
        LOG_ERR("ERRO: falhou em configurar pino %d", PINO_PEDESTRE_NS_R);
    }
    while(1) {
        k_mutex_lock(&semaforo_pedestre_NS, K_FOREVER);
        gpio_pin_set(gpioa_dev, PINO_PEDESTRE_NS_R, 1);
        k_msleep(4000);
        gpio_pin_set(gpioa_dev, PINO_PEDESTRE_NS_R, 0);
        k_mutex_unlock(&semaforo_pedestre_NS);
    }
}

K_THREAD_DEFINE(s_pedestresNS_G, 512, semaforo_pedestres_nortesul_G, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(s_pedestresNS_R, 512, semaforo_pedestres_nortesul_R, NULL, NULL, NULL, 5, 0, 0);


void main(void)
{
    while (1) {
        k_sleep(K_FOREVER);
    }
}