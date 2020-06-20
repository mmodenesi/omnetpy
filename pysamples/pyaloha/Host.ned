//
// A computer in the ALOHAnet network.
//
simple PyHost
{
    parameters:
        @signal[state](type="long");
        @statistic[radioState](source="state";title="Radio state";enum="IDLE=0,TRANSMIT=1";record=vector);
        double txRate @unit(bps);          // transmission rate
        volatile int pkLenBits @unit(b);   // packet length in bits
        volatile double iaTime @unit(s);   // packet interarrival time
        double slotTime @unit(s);          // zero means no slots (pure Aloha)
        double x @unit(m);                 // the x coordinate of the host
        double y @unit(m);                 // the y coordinate of the host
        double idleAnimationSpeed;         // used when there is no packet being transmitted
        double transmissionEdgeAnimationSpeed; // used when the propagation of a first or last bit is visible
        double midTransmissionAnimationSpeed; // used during transmission
        bool controlAnimationSpeed = default(true);
        @display("i=device/pc_s");
}

