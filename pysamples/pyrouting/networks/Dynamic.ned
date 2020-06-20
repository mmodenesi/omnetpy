package networks;

import node.Node;
import builder.NetBuilder;

network Dynamic
{
    submodules:
        builder: NetBuilder {
            nodesFile = "networks/nodes.txt";
            connectionsFile = "networks/connections.txt";
        }
}


