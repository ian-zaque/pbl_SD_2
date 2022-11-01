# Linguagem Assembly

## Problema 1 - Temporizador

### Autores
<div align="justify">
    <li><a href="https://github.com/dlucasafj">@dlucasafj</a></li>
    <li><a href="https://github.com/ian-zaque">@ian-zaque</a></li>
</div>

### Máquina

1. Hardware:
    - Raspberry Pi Zero W;
2. Arquitetura Raspberry:
    - ARMv6;
3. Linguagem de programação: 
    - Assembly;
  

### Instruções

1. Clone o repositório.
    ```sh
    git clone https://github.com/ian-zaque/pbl_SD_1.git
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

3. Execute o arquivo montado.
    ```sh
     sudo ./display
     ```
