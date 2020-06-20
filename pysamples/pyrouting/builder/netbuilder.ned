package builder;

//
// Dynamically sets up a network, with the topology coming from text files
//
simple NetBuilder
{
    parameters:
        @display("i=block/cogwheel_s");
        string nodesFile;
        string connectionsFile;
}
