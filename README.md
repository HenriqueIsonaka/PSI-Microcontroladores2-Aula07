# Atividade Semáforo de Pedestres e veículos

Dupla: 
Henrique Isonaka - NUSP:17972455
Pedro Domingues - NUSP:17918742 

OBS: O VSCODE do Pedro não está funcionando apropriadamente, então, para esta atividade, todos os testes e commits foram feitos a partir do VSCODE do Henrique, mesmo todos os testes tendo sido feitos em dupla.

Link github: https://github.com/HenriqueIsonaka/PSI-Microcontroladores2-Aula07 

Para facilitar a identificação dos semáforos e suas posições, já que há 2 semáforos de cada tipo no cruzamento solicitado,  adotamos os identificadores Norte-Sul e Leste-Oeste. 

Desenho representando visualmente como os semáforos se organizaram na vida real.  Os semáforos que possuem o mesmo identificador (Norte-Sul ou Leste-Oeste), têm os seus tráfegos paralelos.

Foram utilizadas duas placas frdm-kl25z, 2 LEDs vermelhos, 2 LEDs verdes, 4 resistores de 220 ohms, 20 jumpers(estimativa) e 1 protoboard.


**Teste do semáforo de pedestres Norte e Sul** 

- Nesse teste era necessário fazer um código que simulasse um semáforo de pedestres com um microcontrolador que controlasse as luzes dele a partir de duas threads diferentes. Cada luz do semáforo deveria ficar acesa por 4 segundos.

- Foram criadas duas threads: 1 para controlar a luz verde e 1 para controlar a luz vermelha do semáforo. Cada uma dessas threads manda um sinal digital para o pino que controla o LED de sua cor. Ambas as threads funcionam a partir de um looping que liga o LED, espera 4 segundos e desliga o LED.

- A sincronização entre LEDs foi feita a partir do mutex semaforo_pedestre_NS, que impede as duas threads de executarem ao mesmo tempo. Dessa forma, é necessário que uma thread complete o seu looping inteiro para que a outra possa executar,  sendo que, assim, os LEDs do semáforo ficam se revezando para ficarem acesos, igual aos semáforos da vida real.

<ins>Teste:</ins> 
- Pré-condição: Iniciar a execução do código
- Etapas de teste: Observar os LEDs controlados pelos pinos correspondentes ao semáforo de pedestres Norte e Sul.
- Pós-condição: Em nenhum momento os LEDs verde e vermelho ficam simultaneamente apagados ou acesos: sempre que um apaga o outro acende.
- Pós- condição foi atingida.
  
<img width="426" height="240" alt="Semaforo pedestres norte e sul" src="https://github.com/user-attachments/assets/5daedda7-7d18-43db-a264-96425e7b1583" /><img width="426" height="240" alt="Video Project 35" src="https://github.com/user-attachments/assets/d9804eca-8267-43d9-b385-e36939dc57a9" />


**Teste do semáforo de pedestres Leste e Oeste (com microcontrolador secundário)** 

- Nesse teste era necessário fazer um código que simulasse um semáforo de pedestres com um microcontrolador que controlasse as luzes dele a partir de duas threads diferentes. Além disso, seria necessário utilizar um outro microcontrolador para fazer os LEDs acenderem e apagarem. Cada luz do semáforo deveria ficar acesa por 4 segundos.

- Foram utilizados dois microcontroladores: um para mandar um sinal que coordena o acender e apagar dos LEDs (microcontrolador principal), e outro para ler o sinal enviado pelo primeiro microcontrolador e mandá-lo para os LEDs, fazendo com que eles respondam de acordo (microcontrolador secundário). Para que isso fosse possível foi criado um código secundário para o microcontrolador secundário, que está no arquivo MCsecundário.c. Ambos os códigos utilizam uma thread para controlar cada LED, assim como no primeiro teste, sendo que o sinal original de controle vem do microcontrolador principal

- A sincronização entre LEDs foi feita a partir do mutex semaforo_pedestre_LO no código do microcontrolador principal, que impede as duas threads de executarem ao mesmo tempo. Dessa forma, é necessário que uma thread complete o seu looping inteiro para que a outra possa executar,  sendo que, assim, os LEDs do semáforo ficam se revezando para ficarem acesos, igual aos semáforos da vida real. Como o microcontrolador só reage aos comando do microcontrolador principal não foi necessário estabelecer nenhum mecanismo de sincronização em seu código.

<ins>Teste 1:</ins>
- Pré-condição: Iniciar a execução do código
- Etapas de teste: Observar se o microcontrolador secundário é capaz de mudar os estados dos LEDs.
- Pós-condição: Os LEDs são ativados e ativados pelo microcontrolador secundário, o que indica sua eficácia para sua função.
- Pós- condição foi atingida.

<ins>Teste 2:</ins>
- Pré-condição: Iniciar a execução do código
- Etapas de teste: Observar os LEDs controlados pelos pinos correspondentes ao semáforo de pedestres Leste e Oeste.
- Pós-condição: Em nenhum momento os LEDs verde e vermelho ficam simultaneamente apagados ou acesos: sempre que um apaga o outro acende.
- Pós- condição foi atingida.

<img width="426" height="240" alt="Semaforo pedestres leste e oeste" src="https://github.com/user-attachments/assets/594918b0-a939-432c-a0a0-635ef80a05aa" />

