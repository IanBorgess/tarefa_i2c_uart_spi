#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "hardware/pio.h"
#include "matriz_leds.h"
#include <ctype.h>

#define UART_ID uart0 // Seleciona a UART0
#define BAUD_RATE 115200 // Define a taxa de transmissão
#define UART_TX_PIN 0 // Pino GPIO usado para TX
#define UART_RX_PIN 1 // Pino GPIO usado para RX
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define button_a 5
#define button_b 6
#define led_pin_green 11
#define led_pin_blue 12


ssd1306_t ssd; // Inicializa a estrutura do display

// Variáveis globais
static volatile uint a = 1;
static volatile uint32_t last_time_a = 0; // Armazena o tempo do último evento (em microssegundos)
static volatile uint32_t last_time_b = 0; // Armazena o tempo do último evento (em microssegundos)

// Matrizes de configuração para os LEDs WS2812B
Matriz_leds_config matriz0 = {
      
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}},   
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}},   
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}},   
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}}
    };
Matriz_leds_config matriz1 = {
    
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},    
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},    
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},       
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}}
    };
Matriz_leds_config matriz2 = {
    
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}}
    };
Matriz_leds_config matriz3 = {
    
    {{0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}}
    };
Matriz_leds_config matriz4 = {
    
    {{0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}}
    };
Matriz_leds_config matriz5 = {
    
    {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}}
    };
Matriz_leds_config matriz6 = {
    
    {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}}
    };
Matriz_leds_config matriz7 ={
  {{0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}},
  {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}}, 
  {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}}, 
  {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}}, 
  {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}}
};

Matriz_leds_config matriz8 ={
  {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}},
  {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}},
  {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}},
  {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}},
  {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}}
};

Matriz_leds_config matriz9 ={
  {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}},
  {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}},
  {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}},
  {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}},
  {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.5}, {0.0, 0.0, 0.0}}
};

//Ponteiros para as matrizes
Matriz_leds_config *matriz[] = {&matriz0, &matriz1, &matriz2, &matriz3, &matriz4, &matriz5, &matriz6, &matriz7, &matriz8, &matriz9};

static void gpio_irq_handler(uint gpio, uint32_t events);
void setup();
void handle_serial_command(char c, PIO pio, uint sm);

int main()
{

    setup();
    PIO pio = pio0;
    uint sm = configurar_matriz(pio);

    int counter = 0;

    imprimir_desenho(matriz[counter], pio, sm);

    bool cor = true;

            
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_send_data(&ssd); // Atualiza o display


    char command[20];
    while (true)
    {
        if (stdio_usb_connected())
        { // Certifica-se de que o USB está conectado
            char c;
            if (scanf("%c", &c) == 1)
            { // Lê caractere da entrada padrão
                handle_serial_command(c, pio, sm);
            }
        }
        sleep_ms(100);
    }
}

//configuração dos pinos
void setup(){

    stdio_init_all();
    // Inicializa a UART
    uart_init(UART_ID, BAUD_RATE);
  
    // Configura os pinos GPIO para a UART
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); // Configura o pino 0 para TX
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); // Configura o pino 1 para RX
  
     //inicializar o botão de interrupção - GPIO5
     gpio_init(button_a);
     gpio_set_dir(button_a, GPIO_IN);
     gpio_pull_up(button_a);
  
     //inicializar o botão de interrupção - GPIO5
     gpio_init(button_b);
     gpio_set_dir(button_b, GPIO_IN);
     gpio_pull_up(button_b);

     // Configura o GPIO para o botão de interrupção
     gpio_set_irq_enabled_with_callback(button_a, GPIO_IRQ_EDGE_FALL, 1, & gpio_irq_handler);
     gpio_set_irq_enabled_with_callback(button_b, GPIO_IRQ_EDGE_FALL, 1, & gpio_irq_handler);

    //Configura os pinos GPIO para o LED verde e azul
    gpio_init(led_pin_green);
    gpio_set_dir(led_pin_green, GPIO_OUT);
    gpio_init(led_pin_blue);
    gpio_set_dir(led_pin_blue, GPIO_OUT);
  
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);
  
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
  
    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd); // Envia os dados para o display
}

// Trata os comandos recebidos pela serial
void handle_serial_command(char c, PIO pio, uint sm) {
    switch (c) {
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
            ssd1306_draw_string(&ssd, (char[]){c, '\0'}, 64, 32);
            ssd1306_send_data(&ssd); // Envia os dados para o display
            printf("Letra %c!\n", c);
            break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            int counter = c - '0';
            imprimir_desenho(matriz[counter], pio, sm);
            printf("Número %c!\n", c);
            break;

        default:
            printf("Comando inválido: '%c'\n", c);
    }
}

static void gpio_irq_handler(uint gpio, uint32_t events) {
    bool cor = true;
    
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo

    // Obtém o tempo atual em microssegundos
    uint32_t current_time_a = to_us_since_boot(get_absolute_time());
    // Verifica se passou tempo suficiente desde o último evento
    if (current_time_a - last_time_a > 200000) // 200 ms de debouncing
    {
        last_time_a = current_time_a; // Atualiza o tempo do último evento
        if (gpio == button_a) {
            gpio_put(led_pin_green, !gpio_get(led_pin_green)); // Alterna o estado
            printf("Led verde %s\n", gpio_get(led_pin_green) ? "ligado" : "desligado");
            
            // Exibe "Led Verde" no display se o LED verde estiver ligado
            if (gpio_get(led_pin_green)) {
                ssd1306_draw_string(&ssd, "Led Verde", 30, 20);
                ssd1306_send_data(&ssd); // Envia os dados para o display
            } else{
                ssd1306_fill(&ssd, !cor); // Limpa o display
                ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
                ssd1306_send_data(&ssd); // Envia os dados para o display
            }
        }
        a++; // incrementa a variavel de verificação
    }

    // Obtém o tempo atual em microssegundos
    uint32_t current_time_b = to_us_since_boot(get_absolute_time());
    // Verifica se passou tempo suficiente desde o último evento
    if (current_time_b - last_time_b > 200000) // 200 ms de debouncing
    {
        last_time_b = current_time_b; // Atualiza o tempo do último evento
        if (gpio == button_b) {
            gpio_put(led_pin_blue, !gpio_get(led_pin_blue)); // Alterna o estado
            printf("Led azul %s\n", gpio_get(led_pin_blue) ? "ligado" : "desligado");
            
            // Exibe "Led Azul" no display se o LED azul estiver ligado
            if (gpio_get(led_pin_blue)) {
                ssd1306_draw_string(&ssd, "Led Azul", 30, 20);
                ssd1306_send_data(&ssd); // Envia os dados para o display
            } else{
                ssd1306_fill(&ssd, !cor); // Limpa o display
                ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
                ssd1306_send_data(&ssd); // Envia os dados para o display
            }
        }
        a++; // incrementa a variavel de verificação
    }
}