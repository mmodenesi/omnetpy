//
// Simple queueing network: generator + FIFO + sink.
//
network PyFifoNet
{
    submodules:
        gen: PySource {
            parameters:
                @display("p=89,100");
        }
        fifo: PyFifo {
            parameters:
                @display("p=209,100");
        }
        sink: PySink {
            parameters:
                @display("p=329,100");
        }
    connections:
        gen.out --> fifo.in;
        fifo.out --> sink.in;
}


