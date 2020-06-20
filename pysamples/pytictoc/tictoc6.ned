simple PyTxc6
{
    parameters:
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

network Tictoc6
{
    submodules:
        tic: PyTxc6 {
            parameters:
                @display("i=,cyan");
        }
        toc: PyTxc6 {
            parameters:
                @display("i=,gold");
        }
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}
