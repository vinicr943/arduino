Descrição do projeto
Sistema embarcado de monitoramento de luminosidade para ambientes sensíveis à luz (ex.: armazenamento de vinhos). Utiliza um sensor LDR para medir a intensidade luminosa, processa o sinal com normalização e ajuste de curva, e apresenta o resultado em um display LCD 16x2. A interface inclui indicadores visuais (LEDs verde, amarelo e vermelho), feedback simbólico (carinhas no LCD) e alerta sonoro (buzzer). O objetivo é manter o ambiente em níveis de penumbra, reduzindo exposição a luz excessiva que pode degradar compostos orgânicos.


Como usar 

1. Ligação e inicialização
Ao energizar, o sistema exibe uma mensagem de boas-vindas e inicia automaticamente o monitoramento.
2. Leitura da luminosidade
O valor de luz é exibido em percentual no LCD (“Luz: X%”).
A leitura é contínua e já filtrada (média de amostras).
3. Interpretação dos indicadores
LED verde / carinha feliz → ambiente ideal (baixa luminosidade, penumbra)
LED amarelo / carinha séria → nível de atenção
LED vermelho / carinha neutra → luminosidade excessiva (condição inadequada)
4. Alerta sonoro (buzzer)
Ativado automaticamente em níveis amarelo e vermelho
Frequência varia conforme a severidade (mais alta = pior condição)
5. Ação do usuário
Se houver alerta (amarelo/vermelho), reduzir a incidência de luz (fechar ambiente, reposicionar fonte de luz, etc.)
Manter o sistema preferencialmente na faixa verde para preservação ideal


Dependências 
Arduino 
LDR (sensor de luz) + resistor (divisor de tensão)
Display LCD 16x2 
3 LEDs (verde, amarelo, vermelho) + resistores
Buzzer
Protoboard e fios
