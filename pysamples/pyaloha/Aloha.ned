//
// The Aloha network consists of hosts which talk to the central "server" via
// the Aloha or Slotted Aloha protocol
//
network Aloha
{
    parameters:
        int numHosts;  // number of hosts
        double txRate @unit(bps);  // transmission rate
        double slotTime @unit(ms);  // zero means no slots (pure Aloha)
        @display("bgi=background/terrain,s;bgb=1000,1000");
    submodules:
        server: PyServer;
        host[numHosts]: PyHost {
            txRate = txRate;
            slotTime = slotTime;
        }
}
