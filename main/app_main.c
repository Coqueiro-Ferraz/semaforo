//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       _              //
//               _    _       _      _        _     _   _   _    _   _   _        _   _  _   _          //
//           |  | |  |_| |\| |_| |\ |_|   |\ |_|   |_| |_| | |  |   |_| |_| |\/| |_| |  |_| | |   /|    //    
//         |_|  |_|  |\  | | | | |/ | |   |/ | |   |   |\  |_|  |_| |\  | | |  | | | |_ | | |_|   _|_   //
//                                                                                       /              //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
*   Programa básico para controle da placa durante a Jornada da Programação 1
*   Permite o controle das entradas e saídas digitais, entradas analógicas, display LCD e teclado. 
*   Cada biblioteca pode ser consultada na respectiva pasta em componentes
*   Existem algumas imagens e outros documentos na pasta Recursos
*   O código principal pode ser escrito a partir da linha 86
*/

// Área de inclusão das bibliotecas
//-----------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "driver/gpio.h"

#include "HCF_IOTEC.h"   // Vai se tornar HCF_IOTEC
#include "HCF_LCD.h" // Vai se tornar HCF_LCD
#include "HCF_ADC.h"   // Vai se tornar HCF_ADC
#include "HCF_MP.h"   // Vai se tornar HCF_MP
// Incluir HCF_IOT HCF_BT HCF_DHT HCF_ULTRA HCF_RFID HCF_ZMPT HCF_ACS HCF_SERVO HCF_OLED HCF_CAM HCF_SD HCF_LORA
//#include "driver/adc.h"

// Área das macros
//-----------------------------------------------------------------------------------------------------------------------
#define avr_vm 19
#define avr_am 18
#define avr_vd 5

#define ave_vm 23
#define ave_am 22
#define ave_vd 21

#define rua_vm 32
#define rua_am 33
#define rua_vd 25

#define ped_vm 4
#define ped_vd 15

#define bot 34
#define buz 14

// Área de declaração de variáveis e protótipos de funções
//-----------------------------------------------------------------------------------------------------------------------

int estado = 0;
int proximo = 1;
bool flag_pedestre = false;

void IRAM_ATTR gpio_isr_handler(void *arg) {
     flag_pedestre = true;
}


void delay_ms(int tempo)
{
    vTaskDelay(tempo / portTICK_PERIOD_MS); 
}

void pisca(int vezes, int tempo)
{
    for(int i = 0; i<vezes; i++)
    {
        gpio_set_level(ped_vm,0);
        gpio_set_level(buz,0);
        delay_ms(200);
        
        gpio_set_level(buz,1);
        gpio_set_level(ped_vm,1);
        delay_ms(200);
    }
    gpio_set_level(buz,0);
}

// Funções e ramos auxiliares
//-----------------------------------------------------------------------------------------------------------------------
void config()
{
    gpio_reset_pin(avr_vm); gpio_set_direction(avr_vm,GPIO_MODE_OUTPUT);
    gpio_reset_pin(avr_am); gpio_set_direction(avr_am,GPIO_MODE_OUTPUT);
    gpio_reset_pin(avr_vd); gpio_set_direction(avr_vd,GPIO_MODE_OUTPUT);

    gpio_reset_pin(ave_vm); gpio_set_direction(ave_vm,GPIO_MODE_OUTPUT);
    gpio_reset_pin(ave_am); gpio_set_direction(ave_am,GPIO_MODE_OUTPUT);
    gpio_reset_pin(ave_vd); gpio_set_direction(ave_vd,GPIO_MODE_OUTPUT);

    gpio_reset_pin(rua_vm); gpio_set_direction(rua_vm,GPIO_MODE_OUTPUT);
    gpio_reset_pin(rua_am); gpio_set_direction(rua_am,GPIO_MODE_OUTPUT);
    gpio_reset_pin(rua_vd); gpio_set_direction(rua_vd,GPIO_MODE_OUTPUT);

    gpio_reset_pin(ped_vm); gpio_set_direction(ped_vm,GPIO_MODE_OUTPUT);
    gpio_reset_pin(ped_vd); gpio_set_direction(ped_vd,GPIO_MODE_OUTPUT);

    gpio_reset_pin(buz); gpio_set_direction(buz,GPIO_MODE_OUTPUT);

    gpio_reset_pin(bot); gpio_set_direction(bot,GPIO_MODE_INPUT); 
    gpio_set_pull_mode(bot, GPIO_FLOATING);

    gpio_set_intr_type(bot, GPIO_INTR_POSEDGE);
    gpio_install_isr_service(0); 
    gpio_isr_handler_add(bot, gpio_isr_handler, NULL);
    
}

void muda_semaforo(int _1,int _2,int _3,int _4,int _5,int _6,int _7,int _8,int _9,int _10,int _11, int _12)
{
    gpio_set_level(avr_vm,_1);
    gpio_set_level(avr_am,_2);
    gpio_set_level(avr_vd,_3);
    
    gpio_set_level(ave_vm,_4);
    gpio_set_level(ave_am,_5);
    gpio_set_level(ave_vd,_6);
    
    gpio_set_level(rua_vm,_7);
    gpio_set_level(rua_am,_8);
    gpio_set_level(rua_vd,_9);

    gpio_set_level(ped_vm,_10);
    gpio_set_level(ped_vd,_11);
    gpio_set_level(buz,_12);


}

// Programa Principal
//-----------------------------------------------------------------------------------------------------------------------

void app_main(void)
{
    /////////////////////////////////////////////////////////////////////////////////////   Programa principal
    config();

    /////////////////////////////////////////////////////////////////////////////////////   Periféricos inicializados

 

    /////////////////////////////////////////////////////////////////////////////////////   Início do ramo principal                    
    while (1)                                                                                                                         
    {                                                                                                                                 
        //_______________________________________________________________________________________________________________________________________________________ //
        //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  Escreva seu código aqui!!! //

        switch(estado){
            case 0: muda_semaforo(0,0,1,0,0,1,1,0,0,1,0,0); delay_ms(2000); proximo=1; break;
            case 1: muda_semaforo(0,0,1,0,1,0,1,0,0,1,0,0); delay_ms(500); proximo=2; break;
            case 2: muda_semaforo(0,0,1,1,0,0,1,0,0,1,0,0); delay_ms(2000); proximo=3; break;
            case 3: muda_semaforo(0,1,0,1,0,0,1,0,0,1,0,0); delay_ms(500); proximo=4; break;
            case 4: muda_semaforo(1,0,0,1,0,0,0,0,1,1,0,0); delay_ms(2000); proximo=5; break;
            case 5: muda_semaforo(1,0,0,1,0,0,0,1,0,1,0,0); delay_ms(500); proximo=0; break;
            case 6: muda_semaforo(1,0,0,1,0,0,1,0,0,0,1,1); delay_ms(2000); break;
            case 7: muda_semaforo(1,0,0,1,0,0,1,0,0,0,0,0);pisca(3,100); flag_pedestre=false; break;
            default: muda_semaforo(0,1,0,0,1,0,0,1,0,0,0,0); proximo=8; break;
        }
        
        printf("Estado:%d próximo:%d\n",estado, proximo);
        if(!flag_pedestre){
            estado = proximo;
            
        }
        else
        {
            if(estado==3||estado==5)
            {
                estado = 6;
            }
            else if(estado==6)
            {
                estado = 7;
            }
            else
            {
                estado = proximo;
            }
            
        }

        //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  Escreva seu só até aqui!!! //
        //________________________________________________________________________________________________________________________________________________________//
        vTaskDelay(10 / portTICK_PERIOD_MS);    // delay mínimo obrigatório, se retirar, pode causar reset do ESP
    }
    
    // caso erro no programa, desliga o módulo ADC
    adc_limpar();

    /////////////////////////////////////////////////////////////////////////////////////   Fim do ramo principal
    
}
