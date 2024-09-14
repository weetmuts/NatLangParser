# NatLangParser

My first natural language parser from 1991 to be used in a text MUD or interactive fiction game.

```
 Syntax analysator 	av Fredrik Öhrström
   -----------------
   Denna fil är ett frukstansvärt hack. Logiken skrevs på en halvtimme,
   efter att jag kommit på hur den skulle skrivas.
   Den använder sig av en (mini) generativ grammatik a la Chomsky
    följande regler finns:
 	S  = NP VP                               S=sats
 	NP = (det) (A) noun                      NP=nominalfras
 	A  = adj (A)				 VP=verbalfras
 	VP = verb [ PF || [ (NP) (PP) ] ]	 det=formord=a an the
 	PF = [ adj || NP ] 			 PF=predikatsfyllnad
 	PP = prep NP				 PP=prepositionsfras
    ord som finns:
	det = a an the  adj = red blue green good evil beautiful dark
	noun= man woman ball computer rock house forest
	verb= is gives takes looks sings goes stands runs
	prep= to at from in into inside on beside

    Ex på mening: The dark woman gives a red ball to the man
		   An evil man goes into the house.

>The evil red ball runs into the forest.
The evil red ball runs into the forest.
 mem=4242 (the evil red ball runs into the forest.)
            det = the
                adj = evil
                    adj = red
                A = red
            A = evil red
            noun = ball
        NP = the evil red ball
            verb = runs
                prep = into
                    det = the
                    noun = forest
                NP = the forest
            PP = into the forest
        VP = runs into the forest
    S = the evil red ball runs into the forest
```