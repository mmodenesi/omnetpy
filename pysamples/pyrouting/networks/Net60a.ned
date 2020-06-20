package networks;

//
// This network adds one extra connection to the Net60 model (Net60.ned).
//
network Net60a extends Net60
{
    connections:
        rte[55].port++ <--> {delay = default(0.1ms); datarate = default(1Gbps);} <--> rte[0].port++;
}
