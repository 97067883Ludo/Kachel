# Kachel-Arduino

## Pin bezetting van de node mcu

1. D1 -> buzzer
2. D2 -> Tempsensor
3. D3 -> PumpRelay
4. D5 -> Rx -> From display
5. D6 -> Tx -> To display

De pomp schakelt in bij een verschil van 5 °C.
De temperatuur wordt ook verzonden naar een api dit gebeurt elke 5 seconden aangegeven met de variabel: *sendDataDelay*. voor de *api call* is WiFi verbinding nodig. dit wordt opgezet bij het opstarten van het systeem. Als er geen WiFi verbinding kan worden gemaakt wordt het process afgebroken en probeert het systeem dit na 10 minuten nogmaals. Je kunt op het HMI zien of er wifi verbinding is dit is aangeduid met het WiFi icoon.
naast het WiFi icoon staat ook een verbindings icoon. dit duidt aan of de api call's goed zijn aangekomen in de database.
De temperatuur wordt ook verstuurd naar de HMI dit gebeurt elke seconde aangegeven met de variabel: *refreshSensorsDelay*.
Er is een auto en man functie auto functie zorgt ervoor dat de temperatuur automatisch wordt beheerst doormiddel van de bovenstaande 5 gaden verschil. de man functie doet niest meer dan de pomp aanzetten.
