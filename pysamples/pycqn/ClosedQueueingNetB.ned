//
// Closed Queueing Network, variant "B".
//
network ClosedQueueingNetB
{
    parameters:
        int numTandems;
        double sDelay @unit(s);
    submodules:
        tandemQueue[numTandems]: PyTandemQueue {
            parameters:
                sDelay = sDelay;
                ownIndex = index;
            gates:
                in[numTandems];
                out[numTandems];
        }
    connections allowunconnected:
        for i=0..numTandems-1, for j=0..numTandems-1 {
            tandemQueue[i].out[j] --> {  delay = sDelay; } --> tandemQueue[j].in[i] if i!=j;
        }
}


