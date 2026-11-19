#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define PORTA_A DT_NODELABEL(gpioa)
#define PORTA_B DT_NODELABEL(gpiob)
#define LED_G_NODE DT_ALIAS(led0)  // LED verde
#define LED_R_NODE DT_ALIAS(led2)  // LED vermelho
#define PINO_PEDESTRE_NS_G 5 // PTA 5
#define PINO_PEDESTRE_NS_R 4 // PTA 4
#define PINO_PEDESTRE_LO_G 0 // PTB 0
#define PINO_PEDESTRE_LO_R 1 // PTB 1

#define PINO_CARRO_LO_G 9 // PTB 9
#define PINO_CARRO_LO_R 11 // PTB 11

LOG_MODULE_REGISTER(megabrain, LOG_LEVEL_DBG);

const struct device *gpioa_dev = DEVICE_DT_GET(PORTA_A);
const struct device *gpiob_dev = DEVICE_DT_GET(PORTA_B);
static const struct gpio_dt_spec led_G = GPIO_DT_SPEC_GET(LED_G_NODE, gpios);
static const struct gpio_dt_spec led_R = GPIO_DT_SPEC_GET(LED_R_NODE, gpios);

int modo = 1;
const int modo_noturno = 1;
const int modo_normal = 0;

K_MUTEX_DEFINE(semaforo_pedestre_NS);
K_MUTEX_DEFINE(semaforo_pedestre_LO);

K_MUTEX_DEFINE(semaforo_carro_NS_GY);
K_MUTEX_DEFINE(semaforo_carro_NS_GR);
K_MUTEX_DEFINE(semaforo_carro_NS_YR);

K_MUTEX_DEFINE(semaforo_carro_LO_GY);
K_MUTEX_DEFINE(semaforo_carro_LO_GR);
K_MUTEX_DEFINE(semaforo_carro_LO_YR);

K_MUTEX_DEFINE(sincronizacao);
K_MUTEX_DEFINE(paralelosNS);
K_MUTEX_DEFINE(paralelosLO);


K_MUTEX_DEFINE(n_ped_NS);
K_MUTEX_DEFINE(n_ped_LO);
K_MUTEX_DEFINE(n_car_NS);
K_MUTEX_DEFINE(n_car_LO);

void semaforo_pedestres_nortesul_G(void *p1, void *p2, void *p3) {
    k_msleep(1000);//delay para norte e sul não começar como vermelho
    while(1) {
        if(modo == modo_noturno) {
            gpio_pin_set(gpioa_dev, PINO_PEDESTRE_NS_G, 0);
        } else {
        k_sched_lock();
        k_mutex_lock(&semaforo_pedestre_NS, K_FOREVER);
        k_mutex_lock(&sincronizacao, K_FOREVER);
        k_mutex_unlock(&paralelosNS);
        gpio_pin_set(gpioa_dev, PINO_PEDESTRE_NS_G, 1);
        k_msleep(4000);
        gpio_pin_set(gpioa_dev, PINO_PEDESTRE_NS_G, 0);
        k_mutex_unlock(&semaforo_pedestre_NS);
        k_mutex_unlock(&sincronizacao);
        k_mutex_lock(&paralelosNS,K_FOREVER);
        k_sched_unlock();
        }
    }
}

void semaforo_pedestres_nortesul_R (void *p1, void *p2, void *p3) {
    //if(modo == modo_noturno) {
      //  k_msleep(1000);
    //}
    while(1) {
        if(modo == modo_noturno) {
            k_mutex_unlock(&n_ped_LO);
            k_mutex_unlock(&n_car_NS);
            k_mutex_unlock(&n_car_LO);
            gpio_pin_set(gpioa_dev, PINO_PEDESTRE_NS_R, 1);
            k_msleep(1000);
            gpio_pin_set(gpioa_dev, PINO_PEDESTRE_NS_R, 0);
            k_mutex_lock(&n_ped_LO, K_FOREVER);
            k_mutex_lock(&n_car_NS, K_FOREVER);
            k_mutex_lock(&n_car_LO, K_FOREVER);
            k_msleep(1000);
        } else {
        k_sched_lock();
        k_mutex_lock(&semaforo_pedestre_NS, K_FOREVER);
        gpio_pin_set(gpioa_dev, PINO_PEDESTRE_NS_R, 1);
        k_msleep(4000);
        gpio_pin_set(gpioa_dev, PINO_PEDESTRE_NS_R, 0);
        k_mutex_unlock(&semaforo_pedestre_NS);
        k_sched_unlock();
        }
    }
}

void semaforo_pedestres_lesteoeste_G (void *p1, void *p2, void *p3) {
    while(1) {
        if(modo == modo_noturno) {
            gpio_pin_set(gpiob_dev, PINO_PEDESTRE_LO_G, 0);
        } else {
        k_sched_lock();
        k_mutex_lock(&semaforo_pedestre_LO, K_FOREVER);
        k_mutex_lock(&sincronizacao, K_FOREVER);
        k_mutex_unlock(&paralelosLO);
        gpio_pin_set(gpiob_dev, PINO_PEDESTRE_LO_G, 1);
        k_msleep(4000);
        gpio_pin_set(gpiob_dev, PINO_PEDESTRE_LO_G, 0);
        k_mutex_unlock(&semaforo_pedestre_LO);
        k_mutex_unlock(&sincronizacao);
        k_mutex_lock(&paralelosLO, K_FOREVER);
        k_sched_unlock();
        }
    }
}

void semaforo_pedestres_lesteoeste_R (void *p1, void *p2, void *p3) {
    while(1) {
        if(modo == modo_noturno) {
            k_mutex_lock(&n_ped_LO, K_FOREVER);
            gpio_pin_set(gpiob_dev, PINO_PEDESTRE_LO_R, 1);
            k_msleep(1000);
            gpio_pin_set(gpiob_dev, PINO_PEDESTRE_LO_R, 0);
            k_mutex_unlock(&n_ped_LO);
            k_msleep(1000);
        } else {
        k_sched_lock();
        k_mutex_lock(&semaforo_pedestre_LO, K_FOREVER);
        gpio_pin_set(gpiob_dev, PINO_PEDESTRE_LO_R, 1);
        k_msleep(4000);
        gpio_pin_set(gpiob_dev, PINO_PEDESTRE_LO_R, 0);
        k_mutex_unlock(&semaforo_pedestre_LO);
        k_sched_unlock();
        }
    }
}

void semaforo_carros_nortesul_G (void *p1, void *p2, void *p3) {
    k_msleep(500);
    while(1) {
        if(modo == modo_noturno) {
        } else {
        k_sched_lock();
        k_mutex_lock(&semaforo_carro_NS_GY, K_FOREVER);
        k_mutex_lock(&semaforo_carro_NS_GR, K_FOREVER);
        k_mutex_lock(&paralelosNS,K_FOREVER);
        gpio_pin_set_dt(&led_G, 1);
        k_msleep(3000);
        gpio_pin_set_dt(&led_G, 0);
        k_mutex_unlock(&semaforo_carro_NS_GY);
        k_mutex_unlock(&semaforo_carro_NS_GR);
        k_mutex_unlock(&paralelosNS);
        k_sched_unlock();
        }
    }
}

void semaforo_carros_nortesul_Y (void *p1, void *p2, void *p3) {
    k_msleep(500);
    while(1) {
        if(modo == modo_noturno) {
            k_mutex_lock(&n_car_NS, K_FOREVER);
            gpio_pin_set_dt(&led_G, 1);
            gpio_pin_set_dt(&led_R, 1);
            k_msleep(1000);
            gpio_pin_set_dt(&led_G, 0);
            gpio_pin_set_dt(&led_R, 0);
            k_mutex_unlock(&n_car_NS);
            k_msleep(1000);
        } else {
        k_sched_lock();
        k_mutex_lock(&semaforo_carro_NS_YR, K_FOREVER);
        k_mutex_lock(&semaforo_carro_NS_GY, K_FOREVER);
        k_mutex_lock(&paralelosNS,K_FOREVER);
        gpio_pin_set_dt(&led_G, 1);
        gpio_pin_set_dt(&led_R, 1);
        k_msleep(1000);
        gpio_pin_set_dt(&led_G, 0);
        gpio_pin_set_dt(&led_R, 0);
        k_mutex_unlock(&semaforo_carro_NS_YR);
        k_mutex_unlock(&semaforo_carro_NS_GY);
        k_mutex_unlock(&paralelosNS);
        k_sched_unlock();
        }
    }
}

void semaforo_carros_nortesul_R (void *p1, void *p2, void *p3) {
    while(1) {
        if(modo == modo_noturno) {
        } else {
        k_sched_lock();
        k_mutex_lock(&semaforo_carro_NS_GR, K_FOREVER);
        k_mutex_lock(&semaforo_carro_NS_YR, K_FOREVER);
        gpio_pin_set_dt(&led_R, 1);
        k_msleep(4000);
        gpio_pin_set_dt(&led_R, 0);
        k_mutex_unlock(&semaforo_carro_NS_GR);
        k_mutex_unlock(&semaforo_carro_NS_YR);
        k_sched_unlock();
        }
    }
}

void semaforo_carros_lesteoeste_G (void *p1, void *p2, void *p3) {
    while(1) {
        if(modo == modo_noturno) {
        } else {
        k_sched_lock();
        k_mutex_lock(&semaforo_carro_LO_GY, K_FOREVER);
        k_mutex_lock(&semaforo_carro_LO_GR, K_FOREVER);
        k_mutex_lock(&paralelosLO, K_FOREVER);
        gpio_pin_set(gpiob_dev, PINO_CARRO_LO_G, 1);
        k_msleep(3000);
        gpio_pin_set(gpiob_dev, PINO_CARRO_LO_G, 0);
        k_mutex_unlock(&semaforo_carro_LO_GY);
        k_mutex_unlock(&semaforo_carro_LO_GR);
        k_mutex_unlock(&paralelosLO);
        k_sched_unlock();
        }
    }
}

void semaforo_carros_lesteoeste_Y (void *p1, void *p2, void *p3) {
    k_msleep(500);
    while(1) {
        if(modo == modo_noturno) {
            k_mutex_lock(&n_car_LO, K_FOREVER);
            gpio_pin_set(gpiob_dev, PINO_CARRO_LO_G, 1);
            gpio_pin_set(gpiob_dev, PINO_CARRO_LO_R, 1);
            k_msleep(1000);
            gpio_pin_set(gpiob_dev, PINO_CARRO_LO_G, 0);
            gpio_pin_set(gpiob_dev, PINO_CARRO_LO_R, 0);
            k_mutex_unlock(&n_car_LO);
            k_msleep(1000);
        } else {
        k_sched_lock();
        k_mutex_lock(&semaforo_carro_LO_YR, K_FOREVER);
        k_mutex_lock(&semaforo_carro_LO_GY, K_FOREVER);
        k_mutex_lock(&paralelosLO, K_FOREVER);
        gpio_pin_set(gpiob_dev, PINO_CARRO_LO_G, 1);
        gpio_pin_set(gpiob_dev, PINO_CARRO_LO_R, 1);
        k_msleep(1000);
        gpio_pin_set(gpiob_dev, PINO_CARRO_LO_G, 0);
        gpio_pin_set(gpiob_dev, PINO_CARRO_LO_R, 0);
        k_mutex_unlock(&semaforo_carro_LO_YR);
        k_mutex_unlock(&semaforo_carro_LO_GY);
        k_mutex_unlock(&paralelosLO);
        k_sched_unlock();
        }
    }
}

void semaforo_carros_lesteoeste_R (void *p1, void *p2, void *p3) {
    k_msleep(500);
    while(1) {
        if(modo == modo_noturno) {
        } else {
        k_sched_lock();
        k_mutex_lock(&semaforo_carro_LO_GR, K_FOREVER);
        k_mutex_lock(&semaforo_carro_LO_YR, K_FOREVER);
        gpio_pin_set(gpiob_dev, PINO_CARRO_LO_R, 1);
        k_msleep(4000);
        gpio_pin_set(gpiob_dev, PINO_CARRO_LO_R, 0);
        k_mutex_unlock(&semaforo_carro_LO_GR);
        k_mutex_unlock(&semaforo_carro_LO_YR);
        k_sched_unlock();
        }
    }
}

/*
void m_noturno (void *p1, void *p2, void *p3) {
    k_msleep(8000);
    modo = 1;
    k_sleep(K_FOREVER);
}
    */



K_THREAD_DEFINE(s_pedestresNS_G, 512, semaforo_pedestres_nortesul_G, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(s_pedestresNS_R, 512, semaforo_pedestres_nortesul_R, NULL, NULL, NULL, 5, 0, 0);

K_THREAD_DEFINE(s_pedestresLO_G, 512, semaforo_pedestres_lesteoeste_G, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(s_pedestresLO_R, 512, semaforo_pedestres_lesteoeste_R, NULL, NULL, NULL, 5, 0, 0);

K_THREAD_DEFINE(s_carrosNS_G, 512, semaforo_carros_nortesul_G, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(s_carrosNS_Y, 512, semaforo_carros_nortesul_Y, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(s_carrosNS_R, 512, semaforo_carros_nortesul_R, NULL, NULL, NULL, 5, 0, 0);

K_THREAD_DEFINE(s_carrosLO_G, 512, semaforo_carros_lesteoeste_G, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(s_carrosLO_Y, 512, semaforo_carros_lesteoeste_Y, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(s_carrosLO_R, 512, semaforo_carros_lesteoeste_R, NULL, NULL, NULL, 5, 0, 0);

//K_THREAD_DEFINE(modo_n, 512, m_noturno, NULL, NULL, NULL, 3, 0, 0);

void main(void)
{
    int retG, retR, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
    if(!device_is_ready(gpioa_dev)) {
        LOG_ERR("ERRO: Porta A não está pronta");
        return;
    }
    if(!device_is_ready(gpiob_dev)) {
        LOG_ERR("ERRO: Porta B não está pronta");
        return;
    }
    ret1  = gpio_pin_configure(gpioa_dev, PINO_PEDESTRE_NS_G, GPIO_OUTPUT_INACTIVE);
    if(ret1 < 0) {
        LOG_ERR("ERRO: falhou em configurar pino %d", PINO_PEDESTRE_NS_G);
    }
    ret2  = gpio_pin_configure(gpioa_dev, PINO_PEDESTRE_NS_R, GPIO_OUTPUT_INACTIVE);
    if(ret2 < 0) {
        LOG_ERR("ERRO: falhou em configurar pino %d", PINO_PEDESTRE_NS_R);
    }
    ret3  = gpio_pin_configure(gpiob_dev, PINO_PEDESTRE_LO_G, GPIO_OUTPUT_INACTIVE);
    if(ret3 < 0) {
        LOG_ERR("ERRO: falhou em configurar pino %d", PINO_PEDESTRE_LO_G);
    }
    ret4  = gpio_pin_configure(gpiob_dev, PINO_PEDESTRE_LO_R, GPIO_OUTPUT_INACTIVE);
    if(ret4 < 0) {
        LOG_ERR("ERRO: falhou em configurar pino %d", PINO_PEDESTRE_LO_R);
    }
    ret5  = gpio_pin_configure(gpiob_dev, PINO_CARRO_LO_G, GPIO_OUTPUT_INACTIVE);
    if(ret5 < 0) {
        LOG_ERR("ERRO: falhou em configurar pino %d", PINO_CARRO_LO_G);
    }
    ret7  = gpio_pin_configure(gpiob_dev, PINO_CARRO_LO_R, GPIO_OUTPUT_INACTIVE);
    if(ret7 < 0) {
        LOG_ERR("ERRO: falhou em configurar pino %d", PINO_CARRO_LO_R);
    }
    if(!device_is_ready(led_G.port)) {
        LOG_ERR("ERRO: LED verde não está pronto");
        return;
    }
    if (!device_is_ready(led_R.port)) {
        LOG_ERR("ERRO: led vermelho não está pronto");
        return;
    }
    retG = gpio_pin_configure_dt(&led_G, GPIO_OUTPUT_INACTIVE);
    if (retG < 0) {
        LOG_ERR("Error %d: failed to configure LED pin\n", retG);
        return;
    }
    retR = gpio_pin_configure_dt(&led_R, GPIO_OUTPUT_INACTIVE);
    if (retR < 0) {
        LOG_ERR("Error %d: failed to configure LED pin\n", retR);
        return;
    }
    
    while (1) {
        k_sleep(K_FOREVER);
    }
}//