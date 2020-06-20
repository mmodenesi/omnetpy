#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/cdisplaystring.h>


void bind_cDisplayString(pybind11::module &m)
{
    pybind11::class_<omnetpp::cDisplayString> py_cDisplayString(
        m,
        "_cDisplayString",
        R"docstring(
        Represents a display string.

        Display strings are used to control the graphical presentation of network
        elements when a graphical runtime environment is used.

        A display string consist of several named tags, where each tag may have
        several arguments (parameters). The syntax: "tag1=value1,val2,val3;tag2=val4,val5".

        This class facilitates tasks such as adding new tags, adding arguments to
        existing tags, removing tags or replacing arguments. The internal storage
        method allows very fast operation; it will generally be faster than
        direct string manipulation.

        The this class does not know about the meaning or semantics of various
        display string tags, it merely parses the string as data elements separated
        by semicolons, equal signs and commas.

        An example:
        ..code: python

            dispstr = cDisplayString("a=1,2;p=alpha,,3")
            dispstr.insertTag("x");
            dispstr.setTagArg("x", 0, "joe");
            dispstr.setTagArg("x", 2, "jim");
            dispstr.setTagArg("p", 0, "beta");
            EV << dispstr.str();  # result: "x=joe,,jim;a=1,2;p=beta,,3"

        Limitation: a tag may have at most 16 arguments.
        )docstring"
    );

    // constructors
    py_cDisplayString.def(pybind11::init<>());
    py_cDisplayString.def(
        pybind11::init<const char *>(),
        R"docstring(
        Constructor that initializes the object with the given string.

        See parse() for details. Throws an error if there was an error
        parsing the string.
        )docstring",
        pybind11::arg("dispstr")
    );

    py_cDisplayString.def(
        "str",
        &omnetpp::cDisplayString::str,
        R"docstring(
        Returns the display string.
        )docstring"
    );
    py_cDisplayString.def(
        "__str__",
        &omnetpp::cDisplayString::str
    );
    py_cDisplayString.def(
        "__repr__",
        [](const omnetpp::cDisplayString &self)
        {
            std::stringstream ss;
            ss << "cDisplayString(" << self.str() << ")";
            return ss.str();
        }
    );
    py_cDisplayString.def(
        "set",
        &omnetpp::cDisplayString::set,
        R"docstring(
        Same as parse
        )docstring"
    );
    py_cDisplayString.def(
        "parse",
        &omnetpp::cDisplayString::parse,
        R"docstring(
        Sets the display string to the given value. Throws an error if there
        was an error parsing the string.

        If a tag argument contains a "," or ";", it needs to be escaped with
        a backslash to take away its special meaning. Other backslashes are
        are ignored (i.e. not stored). To add a backslash into a tag argument,
        it needs to be duplicated. Example: the 't=foo\,bar\;bs=\\' string
        (i.e. the "t=foo\\,bar\\;bs=\\\\" C++ string constant) will be parsed
        as a "t" tag having the single argument "foo,bar;bs=\".
        )docstring"
    );
    py_cDisplayString.def(
        "updateWith",
        pybind11::overload_cast<const omnetpp::cDisplayString &>(&omnetpp::cDisplayString::updateWith),
        R"docstring(
        Update with the contents of another display string. Corresponding
        elements from the new display string will overwrite existing values.
        )docstring",
        pybind11::arg("ds")
    );
    py_cDisplayString.def(
        "updateWith",
        pybind11::overload_cast<const char *>(&omnetpp::cDisplayString::updateWith),
        R"docstring(
        Update with the contents of another display string. Corresponding
        elements from the new display string will overwrite existing values.
        )docstring",
        pybind11::arg("ds")
    );
    py_cDisplayString.def(
        "containsTag",
        &omnetpp::cDisplayString::containsTag,
        R"docstring(
        Returns `True` if the stored display string contains the given tag.
        )docstring"
    );
    py_cDisplayString.def(
        "getNumArgs",
        pybind11::overload_cast<const char *>(&omnetpp::cDisplayString::getNumArgs, pybind11::const_),
        R"docstring(
        Returns the number of arguments a tag actually has in the display string.

        The count includes empty arguments too. For example,
        for the "x=8,,15,2;y=2" display string getNumArgs("x") returns 4,
        and getNumArgs("y") returns 1. If the display string doesn't
        contain the given tag, 0 is returned.
        )docstring"
    );
    py_cDisplayString.def(
        "getNumArgs",
        pybind11::overload_cast<int>(&omnetpp::cDisplayString::getNumArgs, pybind11::const_),
        R"docstring(
        Returns the number of arguments a tag actually has in the display string.

        The count includes empty arguments too. For example,
        for the "x=8,,15,2;y=2" display string getNumArgs("x") returns 4,
        and getNumArgs("y") returns 1. If the display string doesn't
        contain the given tag, 0 is returned.
        )docstring"
    );
    py_cDisplayString.def(
        "setTagArg",
        pybind11::overload_cast<const char *, int, const char *>(&omnetpp::cDisplayString::setTagArg),
        R"docstring(
        Sets an argument for the given tag. The value may be `None`.
        If index points beyond the last argument, the list of arguments will
        be extended by inserting intervening empty arguments if necessary.

        The result is `True` if the operation was successful. `False` is returned
        if the given tag doesn't exist, or index is invalid (negative or
        greater than the maximum number of arguments, currently 16).
        )docstring",
        pybind11::arg("tagname"), pybind11::arg("index"), pybind11::arg("value")
    );
    py_cDisplayString.def(
        "setTagArg",
        pybind11::overload_cast<const char *, int, long>(&omnetpp::cDisplayString::setTagArg),
        R"docstring(
        Convenience function to set a tag argument to a numeric value. Converts
        'value' to string, then calls setTagArg(const char *, int, const char *).
        )docstring",
        pybind11::arg("tagname"), pybind11::arg("index"), pybind11::arg("value")
    );
    py_cDisplayString.def(
        "setTagArg",
        [](omnetpp::cDisplayString &self, const char *tagname, int index, const double value)
        {
            self.setTagArg(tagname, index, value);
        },
        pybind11::arg("tagname"), pybind11::arg("index"), pybind11::arg("value")
    );
    py_cDisplayString.def(
        "setTagArg",
        pybind11::overload_cast<int, int, const char *>(&omnetpp::cDisplayString::setTagArg),
        R"docstring(
        Sets an argument for the given tag. The value may be None.
        If index points beyond the last argument, the list of arguments will
        be extended by inserting intervening empty arguments if necessary.

        The result is `True` if the operation was successful. `False` is returned
        if the given tag doesn't exist, or index is invalid (negative or
        greater than the maximum number of arguments, currently 16).
        )docstring",
        pybind11::arg("tagindex"), pybind11::arg("index"), pybind11::arg("value")
    );
    py_cDisplayString.def(
        "removeTag",
        pybind11::overload_cast<const char *>(&omnetpp::cDisplayString::removeTag),
        R"docstring(
        Removes the given tag with all its arguments from the display
        string. The result is `True` if the tag was actually deleted
        (it existed before), `False` otherwise.
        )docstring",
        pybind11::arg("tagname")
    );
    py_cDisplayString.def(
        "removeTag",
        pybind11::overload_cast<int>(&omnetpp::cDisplayString::removeTag),
        R"docstring(
        Removes the given tag with all its arguments from the display
        string. The result is `True` if the tag was actually deleted
        (it existed before), `False` otherwise.
        )docstring",
        pybind11::arg("index")
    );
    py_cDisplayString.def(
        "getNumTags",
        &omnetpp::cDisplayString::getNumTags,
        R"docstring(
        Returns the number of tags in the display string. Tags are indexed
        starting from 0.
        )docstring"
    );
    py_cDisplayString.def(
        "getTagName",
        &omnetpp::cDisplayString::getTagName,
        R"docstring(
        Returns the name of the tag given with its index.
        If the tag index is out of range, `None` is returned.
        )docstring",
        pybind11::arg("tagindex")
    );
    py_cDisplayString.def(
        "getTagIndex",
        &omnetpp::cDisplayString::getTagIndex,
        R"docstring(
        Returns the tag index for the given tag, or -1 if the display string
        does not contain the given tag.
        )docstring",
        pybind11::arg("tagname")
    );
    py_cDisplayString.def(
        "getTagArg",
        pybind11::overload_cast<const char *, int>(&omnetpp::cDisplayString::getTagArg, pybind11::const_),
        R"docstring(
        Returns pointer to the indexth argument of the given tag.
        If the tag doesn't exist or the index is out of range,
        empty string ("") is returned.
        )docstring",
        pybind11::arg("tagname"), pybind11::arg("index")
    );
    py_cDisplayString.def(
        "getTagArg",
        pybind11::overload_cast<int, int>(&omnetpp::cDisplayString::getTagArg, pybind11::const_),
        R"docstring(
        Returns the indexth argument of the given tag.
        If the tag doesn't exist or the index is out of range,
        `None` is returned.
        )docstring",
        pybind11::arg("tagindex"), pybind11::arg("index")
    );
    py_cDisplayString.def(
        "insertTag",
        &omnetpp::cDisplayString::insertTag,
        R"docstring(
        Inserts a tag into the display string, optionally at the given
        index. If no index is given, the tag is inserted at the beginning
        of the string. Return value is the index of the tag.
        If the display string already contains a tag with the given tagname,
        nothing is changed and the index of the existing tag is returned.
        )docstring",
        pybind11::arg("tagname"), pybind11::arg("atindex") = 0
    );
}
