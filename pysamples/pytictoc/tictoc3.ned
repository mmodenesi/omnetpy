simple PyTxc3
{
    parameters:
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

//
// Same as Tictoc2.
//
network Tictoc3
{
    submodules:
        tic: PyTxc3 {
            parameters:
                @display("i=,cyan");
        }
        toc: PyTxc3 {
            parameters:
                @display("i=,gold");
        }
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}

