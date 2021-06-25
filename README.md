# EmbeddedSystems
Embedded Systems projects for TIVA C Series TM4C1294XL

## Laboratório 01
 ####### RESULTADOS #######  
 24MHz - Baixa Otimização - Tempo estimado para 5 ciclos = 10,21s  
 120MHz - Baixa Otimização - Tempo estimado para 5 ciclos = 17,30s  
 24MHz - Alta Otimização - Tempo estimado para 5 ciclos = 4,11s  
 120MHz - Alta Otimização - Tempo estimado para 5 ciclos = 10,01s  
 ##########################  

Análise: Observa-se que, para um baixo nível de otimização, o clock de frequência mais alta apresentou um resultado divergente do esperado.
Isso acontece pois existe um delay para o processamento das instruções, que se propaga de forma acumulada conforme um maior número de ciclos é executado.
Isso acabou sendo compensado ao recompilar o código com um alto nível de otimização, tornando o resultado mais próximo ao esperado para o clock de 120MHz, porém desta vez os clocks mais baixos acabaram apresentando divergências por terem adiantado a execução das instruções devido à otimização.
