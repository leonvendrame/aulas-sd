# aulas-sd
Repositório destinado aos códigos de exercícios de aulas de Sistemas Digitais (microcontrolador ATmega328P)

## Compilação
Os códigos podem ser compilados utilizando o __Makefile__.

Como compilar um código com múltiplas dependências:  
```make PROG=activities/a3e1_temp_sensor_to_lcd DEPS="libs/dht11lib libs/displaylcdlib"```

É possível utilizar __*make install*__ para além de compilar, enviar o código para o arduíno.  
```make install PROG=activities/a3e1_temp_sensor_to_lcd DEPS="libs/dht11lib libs/displaylcdlib"```

Note que isso requer o __*avrdude*__ instalado, e que as configurações são para o microcontrolador utilizado em aula.