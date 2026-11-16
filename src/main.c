#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define PORTA_A DT_NODELABEL(gpioa)
#define PORTA_B DT_NODELABEL(gpiob)
#define PINO_PEDESTRE_NS_G 5
#define PINO_PEDESTRE_NS_R 4
#define PINO_PEDESTRE_LO_G 0
#define PINO_PEDESTRE_LO_R 1

LOG_MODULE_REGISTER(megabrain, LOG_LEVEL_DBG);

const struct device *gpioa_dev = DEVICE_DT_GET(PORTA_A);
const struct device *gpiob_dev = DEVICE_DT_GET(PORTA_B);

K_MUTEX_DEFINE(semaforo_pedestre_NS);
K_MUTEX_DEFINE(semaforo_pedestre_LO);

K_MUTEX_DEFINE(sincronizacao);

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
    k_msleep(1000);//delay para norte e sul não começar como vermelho
    while(1) {
        k_mutex_lock(&semaforo_pedestre_NS, K_FOREVER);
        k_mutex_lock(&sincronizacao, K_FOREVER);
        gpio_pin_set(gpioa_dev, PINO_PEDESTRE_NS_G, 1);
        k_msleep(4000);
        gpio_pin_set(gpioa_dev, PINO_PEDESTRE_NS_G, 0);
        k_mutex_unlock(&semaforo_pedestre_NS);
        k_mutex_unlock(&sincronizacao);
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

void semaforo_pedestres_lesteoeste_G (void *p1, void *p2, void *p3) {
    int ret3;
    if(!device_is_ready(gpiob_dev)) {
        LOG_ERR("ERRO: Porta B não está pronta");
        return 1;
    }
    ret3  = gpio_pin_configure(gpiob_dev, PINO_PEDESTRE_LO_G, GPIO_OUTPUT_INACTIVE);
    if(ret3 < 0) {
        LOG_ERR("ERRO: falhou em configurar pino %d", PINO_PEDESTRE_LO_G);
    }
    while(1) {
        k_mutex_lock(&semaforo_pedestre_LO, K_FOREVER);
        k_mutex_lock(&sincronizacao, K_FOREVER);
        gpio_pin_set(gpiob_dev, PINO_PEDESTRE_LO_G, 1);
        k_msleep(4000);
        gpio_pin_set(gpiob_dev, PINO_PEDESTRE_LO_G, 0);
        k_mutex_unlock(&semaforo_pedestre_LO);
        k_mutex_unlock(&sincronizacao);
    }
}

void semaforo_pedestres_lesteoeste_R (void *p1, void *p2, void *p3) {
    int ret4;
    if(!device_is_ready(gpiob_dev)) {
        LOG_ERR("ERRO: Porta B não está pronta");
        return 1;
    }
    ret4  = gpio_pin_configure(gpiob_dev, PINO_PEDESTRE_LO_R, GPIO_OUTPUT_INACTIVE);
    if(ret4 < 0) {
        LOG_ERR("ERRO: falhou em configurar pino %d", PINO_PEDESTRE_LO_R);
    }
    while(1) {
        k_mutex_lock(&semaforo_pedestre_LO, K_FOREVER);
        gpio_pin_set(gpiob_dev, PINO_PEDESTRE_LO_R, 1);
        k_msleep(4000);
        gpio_pin_set(gpiob_dev, PINO_PEDESTRE_LO_R, 0);
        k_mutex_unlock(&semaforo_pedestre_LO);
    }
}


K_THREAD_DEFINE(s_pedestresNS_G, 512, semaforo_pedestres_nortesul_G, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(s_pedestresNS_R, 512, semaforo_pedestres_nortesul_R, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(s_pedestresLO_G, 512, semaforo_pedestres_lesteoeste_G, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(s_pedestresLO_R, 512, semaforo_pedestres_lesteoeste_R, NULL, NULL, NULL, 5, 0, 0);

void main(void)
{
    while (1) {
        k_sleep(K_FOREVER);
    }
}