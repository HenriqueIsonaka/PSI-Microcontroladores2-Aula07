# Atividade Semáforo de Pedestres e veículos

Dupla: 

Henrique Isonaka - NUSP:17972455

Pedro Domingues - NUSP:17918742 

OBS: O VSCODE do Pedro não está funcionando apropriadamente, então, para esta atividade, todos os testes e commits foram feitos a partir do VSCODE do Henrique, mesmo todos os testes tendo sido feitos em dupla.

Link github: https://github.com/HenriqueIsonaka/PSI-Microcontroladores2-Aula07 

Para facilitar a identificação dos semáforos e suas posições, já que há 2 semáforos de cada tipo no cruzamento solicitado,  adotamos os identificadores Norte-Sul e Leste-Oeste. 

Desenho representando visualmente como os semáforos se organizaram na vida real.  Os semáforos que possuem o mesmo identificador (Norte-Sul ou Leste-Oeste), têm os seus tráfegos paralelos.

Foram utilizadas duas placas frdm-kl25z, 2 LEDs vermelhos, 2 LEDs verdes, 4 resistores de 220 ohms, 20 jumpers(estimativa) e 1 protoboard.


## **Teste do semáforo de pedestres Norte e Sul** 

- Nesse teste era necessário fazer um código que simulasse um semáforo de pedestres com um microcontrolador que controlasse as luzes dele a partir de duas threads diferentes. Cada luz do semáforo deveria ficar acesa por 4 segundos.

- Foram criadas duas threads: 1 para controlar a luz verde e 1 para controlar a luz vermelha do semáforo. Cada uma dessas threads manda um sinal digital para o pino que controla o LED de sua cor. Ambas as threads funcionam a partir de um looping que liga o LED, espera 4 segundos e desliga o LED.

- A sincronização entre LEDs foi feita a partir do mutex semaforo_pedestre_NS, que impede as duas threads de executarem ao mesmo tempo. Dessa forma, é necessário que uma thread complete o seu looping inteiro para que a outra possa executar,  sendo que, assim, os LEDs do semáforo ficam se revezando para ficarem acesos, igual aos semáforos da vida real.

<ins>Teste:</ins> 
- Pré-condição: Iniciar a execução do código
- Etapas de teste: Observar os LEDs controlados pelos pinos correspondentes ao semáforo de pedestres Norte e Sul.
- Pós-condição: Em nenhum momento os LEDs verde e vermelho ficam simultaneamente apagados ou acesos: sempre que um apaga o outro acende.
- Pós- condição foi atingida.
  
<img width="426" height="240" alt="Semaforo pedestres norte e sul" src="https://github.com/user-attachments/assets/5daedda7-7d18-43db-a264-96425e7b1583" />


## **Teste do semáforo de pedestres Leste e Oeste (com microcontrolador secundário)** 

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

OBS: O vídeo foi afetado pela compressão, e o LED verde não parece que está piscando quando o vermelho apaga, mas na verdade está sim.

## **Teste de integração dos semáforos de pedestres** 

- Nesse teste era necessário sincronizar os semáforos criados nos testes anteriores para que eles se comportassem como semáforos de pedestres em um cruzamento. Desse modo, considerando que nesse cruzamento haveria também carros sincronizados por semáforos, que impediram a passagem de um dos semáforos de pedestres por vez, seria necessário que eles ficassem sincronizados de modo que um quando um estivesse aberto o outro estivesse fechado.

- A sincronização foi feita a partir do mutex sincronização, que era necessário para que as threads da luz verde de ambos os semáforos de pedestre executassem. Dessa forma, apenas um semáforo de pedestre ficava verde por vez.

<ins>Teste:</ins> 
- Pré-condição: Iniciar a execução do código
- Etapas de teste:  Observar os LEDs controlados pelos pinos correspondentes aos semáforos de pedestres Leste e Oeste e Norte e Sul
- Pós-condição: Em nenhum momento os dois semáforos de pedestre devem estar na mesma cor.
- Pós- condição foi atingida.

<img width="426" height="240" alt="sincroniasemaforos" src="https://github.com/user-attachments/assets/77cbbc8e-53a2-4972-bbca-4cdbfc0c0ea9" />

## **Teste do semáforo de carros Norte e Sul**

- Nesse teste era necessário fazer um código que simulasse um semáforo de carros com um microcontrolador que controlasse as luzes dele a partir de 3 threads diferentes, uma para a luz verde, outra para a luz amarela e outra para a luz vermelha. A luz verde deveria ficar acesa por 3 segundos, a luz amarela deveria ficar acesa por 1 segundo e a luz vermelha deveria ficar acesa por 4 segundos.

- Foram criadas 3 threads: uma para a luz verde, outra para a luz amarela e outra para a luz vermelha. Cada uma dessas threads controla uma configuração do LED do microcontrolador. Todas as threads funcionam a partir de um looping que liga o LED da sua respectiva cor, espera o tempo em que ele deveria ficar aceso e desliga esse LED.

- A sincronização entre LEDs foi feita a partir de 3 mutexes, que impede as 3 threads de executarem ao mesmo tempo e ordena a execução delas, para que sempre o LED amarelo acenda depois do verde, o vermelho acenda depois do amarelo e o verde acenda depois do vermelho. Isso foi feito colocando dois mutexes por thread e ordenando-os de modo que uma thread precise esperar a execução das outras duas de forma ordenada para que ela também possa executar. Dessa forma, os LEDs simulam o comportamento de um semáforo de carros real, que tem uma ordem determinada e nunca fica com duas luzes acesas ao mesmo tempo.

<ins>Teste:</ins> 
- Pré-condição: Iniciar a execução do código
- Etapas de teste:  Observar a cor do LED do microcontrolador.
- Pós-condição: Em nenhum momento o LED da placa fica com duas cores de semáforo sobrepostas, e sempre segue a sequência: vermelho antes do verde, verde antes do amarelo e amarelo antes do vermelho.
- Pós- condição foi atingida.

<img width="426" height="240" alt="Semaforo carros norte e sul" src="https://github.com/user-attachments/assets/36beae96-b6b9-4a3b-af1f-32ec77a67382" />

## **Teste do semáforo de carros Norte e Sul**

- Nesse teste era necessário fazer um código que simulasse um semáforo de carros com um microcontrolador que controlasse as luzes dele a partir de 3 threads diferentes, uma para a luz verde, outra para a luz amarela e outra para a luz vermelha. A luz verde deveria ficar acesa por 3 segundos, a luz amarela deveria ficar acesa por 1 segundo e a luz vermelha deveria ficar acesa por 4 segundos.

- Foram criadas 3 threads: uma para a luz verde, outra para a luz amarela e outra para a luz vermelha. Cada uma dessas threads controla uma configuração do LED do microcontrolador. Todas as threads funcionam a partir de um looping que liga o LED da sua respectiva cor, espera o tempo em que ele deveria ficar aceso e desliga esse LED.

- A sincronização entre LEDs foi feita a partir de 3 mutexes, que impede as 3 threads de executarem ao mesmo tempo e ordena a execução delas, para que sempre o LED amarelo acenda depois do verde, o vermelho acenda depois do amarelo e o verde acenda depois do vermelho. Isso foi feito colocando dois mutexes por thread e ordenando-os de modo que uma thread precise esperar a execução das outras duas de forma ordenada para que ela também possa executar. Dessa forma, os LEDs simulam o comportamento de um semáforo de carros real, que tem uma ordem determinada e nunca fica com duas luzes acesas ao mesmo tempo.

<ins>Teste:</ins> 
- Pré-condição: Iniciar a execução do código
- Etapas de teste:  Observar a cor do LED do microcontrolador.
- Pós-condição: Em nenhum momento o LED da placa fica com duas cores de semáforo sobrepostas, e sempre segue a sequência: vermelho antes do verde, verde antes do amarelo e amarelo antes do vermelho.
- Pós- condição foi atingida.

<img width="426" height="240" alt="Semaforo carros leste e oeste" src="https://github.com/user-attachments/assets/2418b24d-02ad-41fe-80f2-b93e51f4cb05" />

## **Teste de integração dos semáforos de carros** 

- Nesse teste era necessário integrar ambos os semáforos de carros, de modo que eles se comportassem como semáforos de carros em um cruzamento. Dessa forma, seria necessário sincronizá-los de modo que quando um dos semáforos estivesse aberto o outro estivesse fechado.

- A sincronização foi feita a partir do mutex sincronização nas threads dos faróis verdes e amarelos, sendo o bloqueio dela alocado após os bloqueios dos mutexes de sincronização interna dos semáforos. Também foi necessário adicionar um pequeno delay nas threads das luzes verde e amarela do semáforo Norte e Sul e nas threads das luzes vermelha e amarela do semáforo Leste e Oeste, para que o primeiro semáforo verde na execução do código fosse sempre o semáforo Leste e Oeste.

<ins>Teste:</ins> 
- Pré-condição: Iniciar a execução do código
- Etapas de teste:  Observar as cores dos LEDs dos microcontroladores.
- Pós-condição: Em nenhum momento os dois semáforos ficam sinalizados como abertos para travessia ao mesmo tempo, ou seja, ficam ambos verdes, ambos amarelos, ou um verde e outro amarelo.
- Pós- condição foi atingida.

<img width="426" height="240" alt="Sincronia carros" src="https://github.com/user-attachments/assets/6cb947d6-e20e-44f7-9bb9-f7a5609fd198" />

## **Teste de integração dos semáforos de carros e de pedestres** 

- Neste teste era necessário integrar todos os 4 semáforos (2 de pedestres e 2 de carros) de modo que eles se comportassem como semáforos em um cruzamento. Desse modo, seria necessário sincronizá-los para que os pares de semáforo de carros e semáforo de pedestres paralelos ficassem indicando a mesma sinalização (aberto ou fechado para travessia) e que os membros do par perpendicular indicassem a sinalização oposta. 

- A sincronização foi feita pelos mutexes paralelosNS e paralelosLO, que sincronizavam os semáforos de carros com os semáforos de pedestres que eram paralelos a eles para que ambos ficassem sempre acesos com a mesma sinalização. O funcionamento desses mutexes consistia em bloquear a cor verde dos semáforos verde e amarelo dos carros e só liberar a execução delas quando o semáforo de pedestres paralelo a eles também ficasse verde.

<ins>Teste:</ins> 
- Pré-condição: Iniciar a execução do código
- Etapas de teste:  Observar as cores dos LEDs dos microcontroladores e dos LEDs na protoboard.
- Pós-condição: Os pares paralelos de semáforos devem sempre indicar a mesma sinalização, enquanto os semáforos perpendiculares a eles devem sempre indicar a sinalização oposta.
- Pós- condição foi atingida.

<img width="426" height="240" alt="Sincronia geral" src="https://github.com/user-attachments/assets/54c0d39f-1f45-4838-89a5-397ae8eb7edc" />

## **Teste do modo noturno** 

- Foi necessário criar um código independente para implementar o modo noturno do semáforo, no qual todos os LEDs piscam em amarelo (semáforos de carros) ou vermelho (semáforos de pedestres). Para utilizá-lo é necessário mudar a variável global “modo” de 0 para 1, que configurará todas as threads para o modo noturno. Isso foi modificado em versões mais recentes do código, que utilizam #ifdef para selecionar o modo de operação em vez de um if nas threads.

- As threads de luz verde, para ambos os tipos de semáforo, e de luz vermelha, para os semáforos de carros, foram programadas para desligarem seus LEDs. Enquanto isso as threads de luz amarela, para carros, e de luz vermelha, para pedestres, forma programadas para ficarem ligadas por um segundo e desligadas por um segundo, assim piscando em ciclos de 2 segundos.

- A sincronização foi feita a partir de 3 mutexes, que são controlados pela thread do semáforo Norte e Sul dos pedestres. Essa thread só libera esses mutexes, que são necessários para a execução das outras threads,  ao iniciar ou reiniciar a execução do seu looping,  assim, fazendo com que  todas as outras threads pisquem em sincronia com ela.

<ins>Teste 1:</ins> 
- Pré-condição: Iniciar a execução do código
- Etapas de teste:  Observar as cores dos LEDs dos microcontroladores e dos LEDs na protoboard.
- Pós-condição: Os LEDs da placa devem sempre piscar em amarelo e somente os LEDs vermelhos da protoboard podem piscar. 
- Pós- condição foi atingida.


<ins>Teste 2: </ins>
- Pré-condição: Iniciar a execução do código
- Etapas de teste:  Observar as cores dos LEDs dos microcontroladores e dos LEDs na protoboard.
- Pós-condição: Todos os LEDs devem piscar de forma sincronizada.
- Pós- condição foi atingida.

<img width="426" height="240" alt="Modo noturno" src="https://github.com/user-attachments/assets/4569f7e3-f999-4cbe-9ff9-6e24bf670f21" />

## **Teste do botão**

- Foi necessário criar um código independente para implementar o botão dos pedestres. Esse novo código aborda o contexto de uma única rua que possui um semáforo de carros e um semáforo para que os pedestres possam atravessá-la. Enquanto o botão não for acionado, o semáforo de carros permanecerá verde, sendo que assim, nenhum pedestre poderá atravessá-la. Quando esse botão for acionado, o semáforo de carros ficará mais um segundo verde, depois ficará um segundo amarelo, e depois ficará vermelho, momento em que o semáforo de pedestres ficará verde, liberando a travessia. Também foi modificado a forma de escolher o modo de operação do código (Normal, Noturno e Botão), sendo que agora é utilizado #ifdef para escolher o código do modo que será compilado. Basta determinar o valor do #define MODO para escolher o modo que será utilizado. 

-  Foram escolhidos o semáforo Leste e Oeste de pedestres e o semáforo Norte e Sul de carros para a simulação da rua com semáforos para pedestres e carros. Desse modo, serão utilizados o LED do microcontrolador principal e os LEDs da protoboard que recebem o sinal do microcontrolador secundário.

- As threads do semáforo Leste e Oeste de pedestres e do semáforo Norte e Sul de carros foram utilizadas para controlar os LEDs do semáforo de pedestres e do semáforo de carros dessa única rua. Além disso foi implementada uma interrupção ativada pelo pressionamento do botão SW0 do microcontrolador.

- A interrupção libera um semáforo que permite a thread do semáforo verde de carros seguir sua execução, apagando e liberando mutexes para as outras threads das outras luzes executarem. Além disso, a interrupção também é desativada enquanto o ciclo de operação desencadeado pelo botão não voltar para o início, ou seja, quando o semáforo de carros verdes se reacender. Dessa forma, as ativações do botão enquanto ele já tiver sido pressionado em um ciclo serão ignoradas.
- Foram utilizados mutexes para sincronizar internamente as luzes do semáforo de pedestre e do semáforo de carros, e semáforos para sincronizar a execução dos semáforos de carros com os semáforos de pedestres.

<ins>Teste 1:</ins> 
- Pré-condição: Iniciar a execução do código.
- Etapas de teste:  Observar as cores dos LEDs do microcontrolador e dos LEDs na protoboard e não apertar o botão.
- Pós-condição: O LED do microcontrolador permanece verde e o apenas o LED da vermelho da protoboard fica aceso. 
- Pós- condição foi atingida.

<ins>Teste 2:</ins> 
- Pré-condição: Apertar o botão.
- Etapas de teste:  Observar as cores dos LEDs do microcontrolador e dos LEDs na protoboard e não apertar o botão.
- Pós-condição: O LED do microcontrolador deve ficar verde por mais um pouco de tempo, depois ficar amarelo por 1 segundo, depois ficar vermelho por 4 segundos e depois ficar verde novamente. Enquanto isso, o LED vermelho da protoboard deve apagar quando o LED do microcontrolador ficar verde, e, nesse mesmo momento, o LED verde da protoboard deve acender, ficando aceso enquanto o LED do microcontrolador ficar vermelho. Após isso, o LED verde da protoboard deve apagar e o LED verde da protoboard deve se acender.
- Pós- condição foi atingida.

<img width="426" height="240" alt="umapertada" src="https://github.com/user-attachments/assets/6bee3ae7-aa31-48ae-a683-1ec80b65f473" />

<ins>Teste 3:</ins> 
- Pré-condição: Apertar o botão.
- Etapas de teste:  Observar as cores dos LEDs do microcontrolador e dos LEDs na protoboard e apertar o botão enquanto o LED verde da protoboard ainda não acendeu ou está aceso.
- Pós-condição: O LED do microcontrolador deve ficar verde por mais um pouco de tempo, depois ficar amarelo por 1 segundo, depois ficar vermelho por 4 segundos e depois ficar verde novamente. Enquanto isso, o LED vermelho da protoboard deve apagar quando o LED do microcontrolador ficar verde, e, nesse mesmo momento, o LED verde da protoboard deve acender, ficando aceso enquanto o LED do microcontrolador ficar vermelho. Após isso, o LED verde da protoboard deve apagar e o LED verde da protoboard deve se acender. Se o botão não for acionado depois de o LED do microcontrolador ficar verde, os LEDs acesos não devem ser modificados.
- Pós- condição foi atingida.

<img width="426" height="240" alt="apertadas" src="https://github.com/user-attachments/assets/3cdce688-e7fe-41dd-b5eb-1cf0dff21279" />

