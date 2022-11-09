## Problema 2 - Interfaces de E/S

### Autores
<div align="justify">
    <li><a href="https://github.com/dlucasafj">@dlucasafj</a></li>
    <li><a href="https://github.com/ian-zaque">@ian-zaque</a></li>
</div>

### Máquina

1. Hardware:
    - Raspberry Pi Zero W;
    - ESP8266 NodeMcu ESP-12E Module;
2. Arquitetura Raspberry:
    - ARMv6;
3. Linguagens de programação: 
    - Assembly;
    - C;
4. Software:
    - Arduino IDE

### Instruções de uso

1. Clone o repositório.
    ```sh
    git clone https://github.com/ian-zaque/pbl_SD_2.git
    ```

2. Compile e monte o arquivo 'display.s'.
    * Com makefile
        ```sh
            make display
        ```

    * Sem makefile
        ```sh
            as -o display.o display.s
        ```
        ```sh
            ld -o display display.o
        ```

3. Compile e monte o arquivo 'uart.c'.
    * Com makefile
        ```sh
            make uart
        ```

    * Sem makefile
        ```sh
            gcc -uart.c -o uart.run -lwiringPi
        ```

4. Configure o módulo Wifi ESP8266 NodeMcu.
    * No software Arduino IDE:
        <ul>
            <li> Abrir arquivo SD_PBL.ino </li>
            <li> Verificar conexão e porta Wifi do ESP NodeMcu </li>
            <li> Se conectado, carregar sketch no módulo </li>
        </ul

5. Execute o arquivo abaixo gerado.
    ```sh
        sudo ./uart.run
     ```
