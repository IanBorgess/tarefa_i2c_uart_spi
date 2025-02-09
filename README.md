# 🌟 Projeto de Controle de LEDs e Display OLED com Raspberry Pi Pico 🌟

Este projeto utiliza um Raspberry Pi Pico para controlar uma matriz de LEDs WS2812B e um display OLED SSD1306 via I2C. O código também inclui a manipulação de botões para controlar LEDs individuais e a recepção de comandos via UART para interação com o usuário.

## 👨‍💻 Requisitos de Hardware

- **Raspberry Pi Pico**
- **Display OLED SSD1306** (conectado via I2C) 🖥️
- **Matriz de LEDs WS2812B** 🌈
- **Botões** 🎮 para controle dos LEDs
- **LEDs** 💡 verde e azul

## ⚙️ Configuração dos Pinos

- **UART0**:
  - TX: GPIO0
  - RX: GPIO1
- **I2C1**:
  - SDA: GPIO14
  - SCL: GPIO15
- **Botões**:
  - Button A: GPIO6
  - Button B: GPIO5
- **LEDs**:
  - LED Verde: GPIO11
  - LED Azul: GPIO12

## ✨ Funcionalidades

1. **Controle de LEDs WS2812B** 🌈:
   - O código define várias matrizes de configuração para os LEDs, que podem ser selecionadas via comandos seriais.
   - As matrizes são exibidas na matriz de LEDs WS2812B.

2. **Display OLED SSD1306** 🔲:
   - O display OLED é utilizado para exibir mensagens e o estado dos LEDs.
   - O display é atualizado continuamente para refletir o estado atual dos LEDs e os comandos recebidos.

3. **Controle de Botões** 🎮:
   - Os botões A e B são utilizados para alternar o estado dos LEDs verde e azul, respectivamente.
   - A função de debouncing é implementada para evitar leituras incorretas devido a ruídos.

4. **Comunicação Serial** 💻:
   - O projeto recebe comandos via UART para controlar a exibição de letras e números no display OLED e na matriz de LEDs.
   - Comandos válidos incluem letras de 'a' a 'z' 🅰️ e números de '0' a '9' 🔢.

## 🧩 Estrutura do Código

- **main()**: Função principal que inicializa o hardware e entra em um loop infinito para processar comandos e atualizar o display.
- **setup()**: Configura os pinos GPIO, inicializa a UART, I2C, e o display OLED.
- **handle_serial_command()**: Processa os comandos recebidos via UART.
- **gpio_irq_handler()**: Manipula as interrupções geradas pelos botões para alternar o estado dos LEDs.

## 🚀 Como Usar

1. **Compilação e Upload** 📂:
   - Compile o código utilizando o ambiente de desenvolvimento apropriado para o Raspberry Pi Pico (ex: VS Code com extensão Pico).
   - Faça o upload do código para o Raspberry Pi Pico.

2. **Interação** 🤖:
   - Utilize os botões para alternar o estado dos LEDs verde e azul.
   - Envie comandos via UART para exibir letras e números no display OLED e na matriz de LEDs.

## 📝 Exemplo de Comandos

- Envie 'a' para exibir a letra 'a' no display OLED.
- Envie '1' para exibir a matriz de LEDs correspondente ao número 1.

## 📦 Dependências

- **pico-sdk**: SDK oficial do Raspberry Pi Pico.
- **ssd1306.h**: Biblioteca para controle do display OLED SSD1306.
- **font.h**: Biblioteca de fontes para exibição de texto no display OLED.
- **matriz_leds.h**: Biblioteca para controle da matriz de LEDs WS2812B.