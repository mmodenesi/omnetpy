simple PyTic8
{
    parameters:
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

simple PyToc8
{
    parameters:
        @display("i=block/process");
    gates:
        input in;
        output out;
}

network Tictoc8
{
    submodules:
        tic: PyTic8 {
            parameters:
                @display("i=,cyan");
        }
        toc: PyToc8 {
            parameters:
                @display("i=,gold");
        }
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}
