#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed as some functions take vector as arguments


#include <omnetpp.h>


/*
 * Trampoline class to let python implement virtual methods
 */
class PyPredicate : public omnetpp::cTopology::Predicate
{

public:

    bool matches(omnetpp::cModule *module) override
    {
        PYBIND11_OVERLOAD(bool, omnetpp::cTopology::Predicate, matches, module);
    }

};


void bind_cTopology(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cTopology,
        omnetpp::cOwnedObject,
        omnetpp::cNamedObject,
        omnetpp::cObject> py_cTopology(
        m,
        "_cTopology",
        R"docstring(
        Routing support. The cTopology class was designed primarily to support
        routing in telecommunication or multiprocessor networks.

        A cTopology object stores an abstract representation of the network
        in graph form:
         - each cTopology node corresponds to a module (simple or compound), and
         - each cTopology edge corresponds to a link or series of connecting links.

        You can specify which modules (either simple or compound) you want to
        include in the graph. The graph will include all connections among the
        selected modules. In the graph, all nodes are at the same level, there is
        no submodule nesting. Connections which span across compound module
        boundaries are also represented as one graph edge. Graph edges are directed,
        just as module gates are.

        @ingroup Utilities
        @see cTopology::Node, cTopology::Link, cTopology::LinkIn, cTopology::LinkOut
        )docstring"
    );

    pybind11::class_<omnetpp::cTopology::Node> py_cTopology_Node(
        py_cTopology,
        "Node",
        R"docstring(
        Supporting class for cTopology, represents a node in the graph.
        )docstring"
    );

    py_cTopology_Node.def(
        pybind11::init<int>(),
        pybind11::arg("moduleId") = -1
    );


    py_cTopology_Node.def(
        "getModuleId",
        &omnetpp::cTopology::Node::getModuleId,
        R"docstring(
        Returns the ID of the network module to which this node corresponds.
        )docstring"
    );

    py_cTopology_Node.def(
        "getModule",
        &omnetpp::cTopology::Node::getModule,
        R"docstring(
        Returns the pointer to the network module to which this node corresponds.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cTopology_Node.def(
        "getWeight",
        &omnetpp::cTopology::Node::getWeight,
        R"docstring(
        Returns the weight of this node. Weight is used with the
        weighted shortest path finder methods of cTopology.
        )docstring"
    );

    py_cTopology_Node.def(
        "setWeight",
        &omnetpp::cTopology::Node::setWeight,
        R"docstring(
        Sets the weight of this node. Weight is used with the
        weighted shortest path finder methods of cTopology.
        )docstring",
        pybind11::arg("d")
    );

    py_cTopology_Node.def(
        "isEnabled",
        &omnetpp::cTopology::Node::isEnabled,
        R"docstring(
        Returns true of this node is enabled. This has significance
        with the shortest path finder methods of cTopology.
        )docstring"
    );

    py_cTopology_Node.def(
        "enable",
        &omnetpp::cTopology::Node::enable,
        R"docstring(
        Enable this node. This has significance with the shortest path
        finder methods of cTopology.
        )docstring"
    );

    py_cTopology_Node.def(
        "disable",
        &omnetpp::cTopology::Node::disable,
        R"docstring(
        Disable this node. This has significance with the shortest path
        finder methods of cTopology.
        )docstring"
    );

    py_cTopology_Node.def(
        "getNumInLinks",
        &omnetpp::cTopology::Node::getNumInLinks,
        R"docstring(
        Returns the number of incoming links to this graph node.
        )docstring"
    );

    py_cTopology_Node.def(
        "getLinkIn",
        &omnetpp::cTopology::Node::getLinkIn,
        R"docstring(
        Returns ith incoming link of graph node.
        )docstring",
        pybind11::arg("i"),
        pybind11::return_value_policy::reference
    );

    py_cTopology_Node.def(
        "getNumOutLinks",
        &omnetpp::cTopology::Node::getNumOutLinks,
        R"docstring(
        Returns the number of outgoing links from this graph node.
        )docstring"
    );

    py_cTopology_Node.def(
        "getLinkOut",
        &omnetpp::cTopology::Node::getLinkOut,
        R"docstring(
        Returns ith outgoing link of graph node.
        )docstring",
        pybind11::arg("i"),
        pybind11::return_value_policy::reference
    );

    py_cTopology_Node.def(
        "getDistanceToTarget",
        &omnetpp::cTopology::Node::getDistanceToTarget,
        R"docstring(
        Returns the distance of this node to the target node.
        )docstring"
    );

    py_cTopology_Node.def(
        "getNumPaths",
        &omnetpp::cTopology::Node::getNumPaths,
        R"docstring(
        Returns the number of shortest paths towards the target node.
        (There may be several paths with the same length.)
        )docstring"
    );

    py_cTopology_Node.def(
        "getPath",
        &omnetpp::cTopology::Node::getPath,
        R"docstring(
        Returns the next link in the ith shortest paths towards the
        target node. (There may be several paths with the same
        length.)
        )docstring",
        pybind11::arg("i"),
        pybind11::return_value_policy::reference
    );

    /*
    pybind11::class_<omnetpp::cTopology::Link> py_cTopology_Link(
        py_cTopology,
        "Link",
        R"docstring(
        Supporting class for cTopology, represents a link in the graph.
        )docstring"
    );

    py_cTopology_Link.def(
        pybind11::init<double>(),
        pybind11::arg("weight") = 1
    );

    py_cTopology_Link.def(
        "getWeight",
        &omnetpp::cTopology::Link::getWeight,
        R"docstring(
        Returns the weight of this link. Weight is used with the
        weighted shortest path finder methods of cTopology.
        )docstring"
    );

    py_cTopology_Link.def(
        "setWeight",
        &omnetpp::cTopology::Link::setWeight,
        R"docstring(
        Sets the weight of this link. Weight is used with the
        weighted shortest path finder methods of cTopology.
        )docstring",
        pybind11::arg("d")
    );

    py_cTopology_Link.def(
        "isEnabled",
        &omnetpp::cTopology::Link::isEnabled,
        R"docstring(
        Returns true of this link is enabled. This has significance
        with the shortest path finder methods of cTopology.
        )docstring"
    );

    py_cTopology_Link.def(
        "enable",
        &omnetpp::cTopology::Link::enable,
        R"docstring(
        Enables this link. This has significance with the shortest path
        finder methods of cTopology.
        )docstring"
    );

    py_cTopology_Link.def(
        "disable",
        &omnetpp::cTopology::Link::disable,
        R"docstring(
        Disables this link. This has significance with the shortest path
        finder methods of cTopology.
        )docstring"
    );
    */

    pybind11::class_<
        omnetpp::cTopology::LinkIn
        //, omnetpp::cTopology::Link
        > py_cTopology_LinkIn(
        py_cTopology,
        "LinkIn",
        R"docstring(
        Supporting class for cTopology.

        While navigating the graph stored in a cTopology, Node's methods return
        LinkIn and LinkOut objects, which are 'aliases' to Link objects.
        LinkIn and LinkOut provide convenience functions that return the
        'local' and 'remote' end of the connection when traversing the topology.
        )docstring"
    );

    py_cTopology_LinkIn.def(
        "getRemoteNode",
        &omnetpp::cTopology::LinkIn::getRemoteNode,
        R"docstring(
        Returns the node at the remote end of this connection.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cTopology_LinkIn.def(
        "getLocalNode",
        &omnetpp::cTopology::LinkIn::getLocalNode,
        R"docstring(
        Returns the node at the local end of this connection.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cTopology_LinkIn.def(
        "getRemoteGateId",
        &omnetpp::cTopology::LinkIn::getRemoteGateId,
        R"docstring(
        Returns the gate ID at the remote end of this connection.
        )docstring"
    );

    py_cTopology_LinkIn.def(
        "getLocalGateId",
        &omnetpp::cTopology::LinkIn::getLocalGateId,
        R"docstring(
        Returns the gate ID at the local end of this connection.
        )docstring"
    );

    py_cTopology_LinkIn.def(
        "getRemoteGate",
        &omnetpp::cTopology::LinkIn::getRemoteGate,
        R"docstring(
        Returns the gate at the remote end of this connection.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cTopology_LinkIn.def(
        "getLocalGate",
        &omnetpp::cTopology::LinkIn::getLocalGate,
        R"docstring(
        Returns the gate at the local end of this connection.
        )docstring",
        pybind11::return_value_policy::reference
    );

    pybind11::class_<
        omnetpp::cTopology::LinkOut
        //, omnetpp::cTopology::Link
        > py_cTopology_LinkOut(
        py_cTopology,
        "LinkOut",
        R"docstring(
        Supporting class for cTopology.

        While navigating the graph stored in a cTopology, Node's methods return
        LinkIn and LinkOut objects, which are 'aliases' to Link objects.
        LinkIn and LinkOut provide convenience functions that return the
        'local' and 'remote' end of the connection when traversing the topology.
        )docstring"
    );

    py_cTopology_LinkOut.def(
        "getRemoteNode",
        &omnetpp::cTopology::LinkOut::getRemoteNode,
        R"docstring(
        Returns the node at the remote end of this connection.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cTopology_LinkOut.def(
        "getLocalNode",
        &omnetpp::cTopology::LinkOut::getLocalNode,
        R"docstring(
        Returns the node at the local end of this connection.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cTopology_LinkOut.def(
        "getRemoteGateId",
        &omnetpp::cTopology::LinkOut::getRemoteGateId,
        R"docstring(
        Returns the gate ID at the remote end of this connection.
        )docstring"
    );

    py_cTopology_LinkOut.def(
        "getLocalGateId",
        &omnetpp::cTopology::LinkOut::getLocalGateId,
        R"docstring(
        Returns the gate ID at the local end of this connection.
        )docstring"
    );

    py_cTopology_LinkOut.def(
        "getRemoteGate",
        &omnetpp::cTopology::LinkOut::getRemoteGate,
        R"docstring(
        Returns the gate at the remote end of this connection.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cTopology_LinkOut.def(
        "getLocalGate",
        &omnetpp::cTopology::LinkOut::getLocalGate,
        R"docstring(
        Returns the gate at the local end of this connection.
        )docstring",
        pybind11::return_value_policy::reference
    );

    pybind11::class_<
        omnetpp::cTopology::Predicate,
        PyPredicate
        > py_cTopology_Predicate(
        py_cTopology,
        "Predicate",
        R"docstring(
        Base class for selector objects used in extract...() methods of cTopology.

        Redefine the matches() method to return whether the given module
        should be included in the extracted topology or not.
        )docstring"
    );

    py_cTopology_Predicate.def(
        "matches",
        &omnetpp::cTopology::Predicate::matches,
        pybind11::arg("module")
    );

    py_cTopology.def(
        pybind11::init<const char*>(),
        pybind11::arg("name") = nullptr
    );

    py_cTopology.def(
        "str",
        &omnetpp::cTopology::str,
        R"docstring(
        Produces a one-line description of the object's contents.
        See cObject for more details.
        )docstring"
    );

    py_cTopology.def(
        "__str__",
        &omnetpp::cTopology::str,
        R"docstring(
        Returns true if histogram is already available. See setUpBins().
        )docstring"
    );

    py_cTopology.def(
        "extractFromNetwork",
        pybind11::overload_cast<omnetpp::cTopology::Predicate *>(&omnetpp::cTopology::extractFromNetwork),
        R"docstring(
        The type safe, object-oriented equivalent of extractFromNetwork(selfunc, userdata).
        )docstring",
        pybind11::arg("predicate")
    );

    py_cTopology.def(
        "extractByModulePath",
        &omnetpp::cTopology::extractByModulePath,
        R"docstring(
        Extracts model topology by module full path. All modules whole getFullPath()
        matches one of the patterns in given string vector will get included.
        The patterns may contain wilcards in the same syntax as in ini files.

        An example:

        ``topo.extractByModulePath("**.host[*] **.router*".split()``
        )docstring",
        pybind11::arg("fullPathPatterns")
    );

    py_cTopology.def(
        "extractByNedTypeName",
        &omnetpp::cTopology::extractByNedTypeName,
        R"docstring(
        Extracts model topology by the fully qualified NED type name of the
        modules. All modules whose getNedTypeName() is listed in the given string
        vector will get included.
        )docstring",
        pybind11::arg("nedTypeNames")
    );

    py_cTopology.def(
        "extractByProperty",
        &omnetpp::cTopology::extractByProperty,
        R"docstring(
        Extracts model topology by a module property. All modules get included
        that have a property with the given name and the given value
        (more precisely, the first value of its default key being the specified
        value). If value is nullptr, the property's value may be anything except
        "false" (i.e. the first value of the default key may not be "false").

        For example, <tt>topo.extractByProperty("node");</tt> would extract
        all modules that contain the <tt>@node</tt> property, like the following
        one:

        <pre>
        module X {
            @node;
        }
        </pre>
        )docstring",
        pybind11::arg("propertyName"), pybind11::arg("value") = nullptr
    );

    py_cTopology.def(
        "extractByParameter",
        &omnetpp::cTopology::extractByParameter,
        R"docstring(
        Extracts model topology by a module parameter. All modules get included
        that have a parameter with the given name, and the parameter's str()
        method returns the paramValue string. If paramValue is nullptr, only the
        parameter's existence is checked but not its value.
        )docstring",
        pybind11::arg("paramName"), pybind11::arg("paramValue") = nullptr
    );

    py_cTopology.def(
        "clear",
        &omnetpp::cTopology::clear,
        R"docstring(
        Deletes the topology stored in the object.
        )docstring"
    );

    py_cTopology.def(
        "addNode",
        &omnetpp::cTopology::addNode,
        R"docstring(
        Adds the given node to the graph. Returns the index of the new graph node
        (see getNode(int)). Indices of existing graph nodes may change.
        )docstring"
    );

    py_cTopology.def(
        "deleteNode",
        &omnetpp::cTopology::deleteNode,
        R"docstring(
        Removes the given node from the graph, together with all of its links.
        Indices of existing graph nodes may change.
        )docstring",
        pybind11::arg("node")
    );

    py_cTopology.def(
        "addLink",
        pybind11::overload_cast<omnetpp::cTopology::Link*, omnetpp::cTopology::Node*, omnetpp::cTopology::Node*>(&omnetpp::cTopology::addLink),
        R"docstring(
        Adds the given link to the graph, between the specified nodes.
        If the link is already part of the graph it is removed first,
        i.e. this method also serves as reconnectLink().
        )docstring",
        pybind11::arg("link"), pybind11::arg("srcNode"), pybind11::arg("destNode")
    );

    py_cTopology.def(
        "addLink",
        pybind11::overload_cast<omnetpp::cTopology::Link*, omnetpp::cGate*, omnetpp::cGate*>(&omnetpp::cTopology::addLink),
        R"docstring(
        Adds the given link to the graph, between the nodes that correspond
        to the specified gates. It is an error if the modules that contain
        the gates are not parts of the graph. If the link is already part
        of the graph it is removed first, i.e. this method also serves as
        reconnectLink().
        )docstring",
        pybind11::arg("link"), pybind11::arg("srcGate"), pybind11::arg("destGate")
    );

    py_cTopology.def(
        "deleteLink",
        &omnetpp::cTopology::deleteLink,
        R"docstring(
        Removes the given link from the graph. Indices of existing links in the
        source and destination nodes may change.
        )docstring",
        pybind11::arg("link")
    );

    py_cTopology.def(
        "getNumNodes",
        &omnetpp::cTopology::getNumNodes,
        R"docstring(
        Returns the number of nodes in the graph.
        )docstring"
    );

    py_cTopology.def(
        "getNode",
        &omnetpp::cTopology::getNode,
        R"docstring(
        Returns pointer to the ith node in the graph. Node's methods
        can be used to further examine the node's connectivity, etc.
        )docstring",
        pybind11::return_value_policy::reference,
        pybind11::arg("i")
    );

    py_cTopology.def(
        "getNodeFor",
        &omnetpp::cTopology::getNodeFor,
        R"docstring(
        Returns the graph node which corresponds to the given module in the
        network. If no graph node corresponds to the module, the method returns
        nullptr. This method assumes that the topology corresponds to the
        network, that is, it was probably created with one of the
        extract...() functions.
        )docstring",
        pybind11::return_value_policy::reference,
        pybind11::arg("mod")
    );

    py_cTopology.def(
        "calculateUnweightedSingleShortestPathsTo",
        &omnetpp::cTopology::calculateUnweightedSingleShortestPathsTo,
        R"docstring(
        Apply the Dijkstra algorithm to find all shortest paths to the given
        graph node. The paths found can be extracted via Node's methods.
        )docstring",
        pybind11::arg("target")
    );

    py_cTopology.def(
        "calculateWeightedSingleShortestPathsTo",
        &omnetpp::cTopology::calculateWeightedSingleShortestPathsTo,
        R"docstring(
        Apply the Dijkstra algorithm to find all shortest paths to the given
        graph node. The paths found can be extracted via Node's methods.
        Uses weights in nodes and links.
        )docstring",
        pybind11::arg("target")
    );

    py_cTopology.def(
        "getTargetNode",
        &omnetpp::cTopology::getTargetNode,
        R"docstring(
        Returns the node that was passed to the most recently called
        shortest path finding function.
        )docstring",
        pybind11::return_value_policy::reference
    );
}
