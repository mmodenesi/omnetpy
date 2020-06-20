simple PyTxc7
{
    parameters:
        volatile double delayTime @unit(s);   // delay before sending back message
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

network Tictoc7
{
    submodules:
        tic: PyTxc7 {
            parameters:
                @display("i=,cyan");
        }
        toc: PyTxc7 {
            parameters:
                @display("i=,gold");
        }
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}

