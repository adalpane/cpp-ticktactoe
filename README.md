# cpp-tictactoe

## Sfida tra strategie
Posizionare le strategie nella directory `strategies`, solamente il corpo della funzione con signature:

```c
Move strategy(const Board &b)
```

Eseguire la battaglia tramite lo script python:

```shell
python -c 'import trisbattle; trisbattle.tournament(10)'
```

Lo script produce in output il risultato di un certo numero di partite (10 nell'esempio), specificando le vittorie ed 
i pareggi dello scontro di tutte le coppie di strategie nella cartella. 