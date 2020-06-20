#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <omnetpp.h>
#include <omnetpp/ccanvas.h>

void bind_cFigure(pybind11::module &);
void bind_cGroupFigure(pybind11::module &);
void bind_cPanelFigure(pybind11::module &);

void bind_cAbstractLineFigure(pybind11::module &);
void bind_cLineFigure(pybind11::module &);
void bind_cArcFigure(pybind11::module &);
void bind_cPolylineFigure(pybind11::module &);

void bind_cAbstractShapeFigure(pybind11::module &);
void bind_cRectangleFigure(pybind11::module &);
void bind_cOvalFigure(pybind11::module &);
void bind_cRingFigure(pybind11::module &);
void bind_cPieSliceFigure(pybind11::module &);
void bind_cPolygonFigure(pybind11::module &);
void bind_cPathFigure(pybind11::module &);

void bind_cAbstractTextFigure(pybind11::module &);
void bind_cTextFigure(pybind11::module &);
void bind_cLabelFigure(pybind11::module &);

void bind_cAbstractImageFigure(pybind11::module &);
void bind_cImageFigure(pybind11::module &);
void bind_cIconFigure(pybind11::module &);
void bind_cPixmapFigure(pybind11::module &);

void bind_cCanvas(pybind11::module &m)
{
    bind_cFigure(m);

    bind_cGroupFigure(m);
    bind_cPanelFigure(m);

    bind_cAbstractLineFigure(m);
    bind_cLineFigure(m);
    bind_cPolylineFigure(m);

    bind_cAbstractShapeFigure(m);
    bind_cRectangleFigure(m);
    bind_cOvalFigure(m);
    bind_cArcFigure(m);
    bind_cRingFigure(m);
    bind_cPieSliceFigure(m);
    bind_cPolygonFigure(m);
    bind_cPathFigure(m);

    bind_cAbstractTextFigure(m);
    bind_cTextFigure(m);
    bind_cLabelFigure(m);

    bind_cAbstractImageFigure(m);
    bind_cImageFigure(m);
    bind_cIconFigure(m);
    bind_cPixmapFigure(m);

    pybind11::class_<omnetpp::cCanvas, omnetpp::cOwnedObject> py_cCanvas(
        m,
        "_cCanvas",
        R"docstring(
        Provides a scene graph based 2D drawing API for modules.

        Notes:
        - all figures are under a root figure (which we try not to expose too much)
        - Z-order is defined by child order (bottom-to-top) and preorder traversal
        - layer-like functionality is provided via figure tags: the Tkenv UI will allow figures
          to be shown or hidden according to tags they contain
        - cModule has one dedicated canvas, additional canvas objects can be created
        - initial module canvas contents comes from @figure properties inside the compound module NED file, see samples/canvas for examples!
        - extensibility: type=foo in a @figure property will cause the canvas to look for a registered FooFigure or cFooFigure class to instantiate
        - dup() makes deep copy (duplicates the figure tree too)
        - the submodules layer (see getSubmodulesLayer()) is currently an empty placeholder figure where Tkenv will
          draw modules and connections (by means outside cCanvas/cFigure) -- it can be used for Z-order positioning
          of other figures relative to the submodules and connections
        )docstring"
    );

    py_cCanvas.def(
        pybind11::init<const char *>(),
        pybind11::arg("name") = nullptr
    );

    py_cCanvas.def(
        "dup",
        &omnetpp::cCanvas::dup
    );

    py_cCanvas.def(
        "__str__",
        &omnetpp::cCanvas::str
    );

    py_cCanvas.def(
        "str",
        &omnetpp::cCanvas::str
    );

    py_cCanvas.def(
        "getBackgroundColor",
        &omnetpp::cCanvas::getBackgroundColor,
        R"docstring(
        Returns the background color of the canvas.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cCanvas.def(
        "setBackgroundColor",
        &omnetpp::cCanvas::setBackgroundColor,
        R"docstring(
        Sets the background color of the canvas.
        )docstring",
        pybind11::arg("color")
    );

    py_cCanvas.def(
        "getRootFigure",
        &omnetpp::cCanvas::getRootFigure,
        R"docstring(
        Returns the root figure of the canvas. The root figure has no visual
        appearance, it solely serves as a container for other figures.
        )docstring",
        pybind11::return_value_policy::reference
    );
    py_cCanvas.def(
        "addFigure",
        pybind11::overload_cast<omnetpp::cFigure *>(&omnetpp::cCanvas::addFigure),
        R"docstring(
        Appends the given figure to the child list of the root figure.
        )docstring",
        pybind11::arg("figure")
    );

    py_cCanvas.def(
        "addFigure",
        pybind11::overload_cast<omnetpp::cFigure *, int>(&omnetpp::cCanvas::addFigure),
        R"docstring(
        Inserts the given figure into the child list of the root figure at
        the given position. Note that relative order in the child list
        only affects stacking order if the respective figures have the
        same Z-index.
        )docstring",
        pybind11::arg("figure"), pybind11::arg("pos")
    );

    py_cCanvas.def(
        "removeFigure",
        pybind11::overload_cast<omnetpp::cFigure *>(&omnetpp::cCanvas::removeFigure),
        R"docstring(
        Removes the given figure from the child list of the root figure.
        An error is raised if the figure is not a child of the root figure.
        )docstring",
        pybind11::arg("figure")
    );

    py_cCanvas.def(
        "removeFigure",
        pybind11::overload_cast<int>(&omnetpp::cCanvas::removeFigure),
        R"docstring(
        Removes the kth figure from the child list of the root figure. An error
        is raised if pos is not in the range 0..getNumFigures()-1.
        )docstring",
        pybind11::arg("pos")
    );

    py_cCanvas.def(
        "findFigure",
        pybind11::overload_cast<const char *>(&omnetpp::cCanvas::findFigure, pybind11::const_),
        R"docstring(
        Finds the first figure with the given name among the children of
        the root figure, and returns its index. If there is no such figure,
        -1 is returned.
        )docstring",
        pybind11::arg("name")
    );

    py_cCanvas.def(
        "findFigure",
        pybind11::overload_cast<omnetpp::cFigure *>(&omnetpp::cCanvas::findFigure, pybind11::const_),
        R"docstring(
        Removes the kth figure from the child list of the root figure. An error
        is raised if pos is not in the range 0..getNumFigures()-1.
        )docstring",
        pybind11::arg("figure")
    );

    py_cCanvas.def(
        "hasFigures",
        &omnetpp::cCanvas::hasFigures,
        R"docstring(
        Returns true if this canvas contains any figures (apart from the
        root figure), and false otherwise.
        )docstring"
    );

    py_cCanvas.def(
        "getNumFigures",
        &omnetpp::cCanvas::getNumFigures,
        R"docstring(
        Returns the number of the root figure's child figures.
        )docstring"
    );

    py_cCanvas.def(
        "getFigure",
        pybind11::overload_cast<int>(&omnetpp::cCanvas::getFigure, pybind11::const_),
        R"docstring(
        Returns the kth figure in the child list of the root figure. The
        index must be in the range 0..getNumFigures()-1. An out-of-bounds
        index will cause a runtime error.
        )docstring",
        pybind11::arg("pos"),
        pybind11::return_value_policy::reference
    );

    py_cCanvas.def(
        "getFigure",
        pybind11::overload_cast<const char *>(&omnetpp::cCanvas::getFigure, pybind11::const_),
        R"docstring(
        Returns the first child figure of the root figure with the given name,
        or nullptr if there is no such figure.
        )docstring",
        pybind11::arg("name"),
        pybind11::return_value_policy::reference
    );

    py_cCanvas.def(
        "getSubmodulesLayer",
        &omnetpp::cCanvas::getSubmodulesLayer,
        R"docstring(
        Returns the figure that (conceptionally) contains the figures
        displaying the submodules and connection arrows in the compound
        module. It may be used for positioning figures in Z order relative
        to submodules.  This method returns nullptr for extra canvases that
        don't display submodules.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cCanvas.def(
        "findFigureRecursively",
        &omnetpp::cCanvas::findFigureRecursively,
        R"docstring(
        Returns the first figure with the given name in the figure tree of
        the canvas, or nullptr if there is no such figure.
        )docstring",
        pybind11::arg("name"),
        pybind11::return_value_policy::reference
    );

    py_cCanvas.def(
        "getFigureByPath",
        &omnetpp::cCanvas::getFigureByPath,
        R"docstring(
        Returns the figure identified by the given path, a string of figure
        names separated by dots. See cFigure::getFigureByPath().
        )docstring",
        pybind11::arg("path"),
        pybind11::return_value_policy::reference
    );

    py_cCanvas.def(
        "getAllTags",
        &omnetpp::cCanvas::getAllTags,
        R"docstring(
        Returns the union of the tags used by figures in this canvas as a
        single space-separated string.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cCanvas.def(
        "getAllTagsAsVector",
        &omnetpp::cCanvas::getAllTagsAsVector,
        R"docstring(
        Returns the union of the tags used by figures in this canvas as a
        vector of strings.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cCanvas.def(
        "setAnimationSpeed",
        &omnetpp::cCanvas::setAnimationSpeed,
        R"docstring(
        Sets the preferred animation speed associated with the given source
        object. Specify zero or a negative speed value to clear (withdraw)
        a previously set animation speed request. The source object only
        serves as a key to allow clients register several animation speed
        requests with the option to later update and clear any of them.

        The UI of the simulation will take these requests into account when
        determining the speed used for actual animation. (For example, the
        UI may take the minimum of the requested animation speeds across all
        currently viewed canvases, or it may ignore the requests and use an
        interactively given animation speed.)
        )docstring",
        pybind11::arg("animationSpeed"), pybind11::arg("source")
    );

    py_cCanvas.def(
        "fetAnimationSpeed",
        &omnetpp::cCanvas::getAnimationSpeed,
        R"docstring(
        Returns the animation speed request associated with the given
        source object, or 0 if none exists.
        )docstring"
    );

    py_cCanvas.def(
        "holdSimulationFor",
        &omnetpp::cCanvas::holdSimulationFor,
        R"docstring(
        Requests a "hold" interval of the given length, starting at the
        current animation time. During a hold interval, only animation
        takes place, but simulation time does not progress and no events
        are processed. Hold intervals are intended for animating actions
        that take zero simulation time.

        If a hold request is issued when there is one already in progress,
        the current hold will be extended as needed to incorporate the
        request. A hold request cannot be cancelled or shrunk.

        When rendering (i.e. inside refreshDisplay()), use
        `self.getEnvir().getRemainingAnimationHoldTime()` or
        `self.getEnvir().getAnimationTime()` to query the
        progress of the animation.
        )docstring",
        pybind11::arg("animationTimeDelta")
    );
}

void bind_cFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cFigure, omnetpp::cOwnedObject,
        std::unique_ptr<omnetpp::cFigure, pybind11::nodelete>
        > py_cFigure(
        m,
        "_cFigure",
        R"docstring(
        A lightweight graphical object for cCanvas.

        This is the central class of \opp's 2D drawing API. Graphical elements
        are called figures, and are represented by subclasses of cFigure.
        To display a figure-based drawing, one needs to add figures to a canvas
        (cCanvas). Every module has its own canvas, accessible via
        cModule::getCanvas(), and one may also create additional ones.

        Notes:
        - figures can be hierarchical (resulting in a figure tree)
        - coordinates are in canvas units, not in pixels (interactive zooming!)
        - coordinates are transformed with the figure's own transform plus the transforms of all ancestors
        - a figure subtree can be hidden by calling setVisible(false)
        - the order of child figures also denotes drawing order (of overlapping figures, the last one will appear on top)
        - dup() makes shallow copy (doesn't copy child figures); see dupTree() as well
        - figures are only data storage classes, rendering is done in the back-end (e.g. Tkenv)
          by separate renderer classes.
        )docstring"
    );

    pybind11::class_<omnetpp::cFigure::Point> py_cFigure_Point(
        py_cFigure,  // note: defined in the scope of cFigure
        "Point",
        R"docstring(
        Represents a point as (x,y) coordinates.
        )docstring"
    );

    py_cFigure_Point.def(pybind11::init<>());
    py_cFigure_Point.def(pybind11::init<double, double>(), pybind11::arg("x"), pybind11::arg("y"));
    py_cFigure_Point.def("__str__", &omnetpp::cFigure::Point::str);
    py_cFigure_Point.def_readwrite("x", &omnetpp::cFigure::Point::x);
    py_cFigure_Point.def_readwrite("y", &omnetpp::cFigure::Point::y);
    py_cFigure_Point.def(pybind11::self + pybind11::self);
    py_cFigure_Point.def(pybind11::self - pybind11::self);
    py_cFigure_Point.def(pybind11::self * pybind11::self);
    py_cFigure_Point.def(pybind11::self == pybind11::self);
    py_cFigure_Point.def(pybind11::self * float());
    py_cFigure_Point.def(pybind11::self / float());
    py_cFigure_Point.def("distanceTo", &omnetpp::cFigure::Point::distanceTo, pybind11::arg("p"));
    py_cFigure_Point.def("getLength", &omnetpp::cFigure::Point::getLength);
    py_cFigure_Point.def("translate", &omnetpp::cFigure::Point::translate, pybind11::arg("dx"), pybind11::arg("dy"));
    py_cFigure_Point.def("str", &omnetpp::cFigure::Point::str);

    pybind11::class_<omnetpp::cFigure::Transform> py_cFigure_Transform(
        py_cFigure,
        "Transform",
        R"docstring(
         Homogeneous 2D transformation matrix.

         Note that the last row is not stored.

          | a  c  t1 |
          | b  d  t2 |
          | 0  0   1 |

         )docstring"
    );

    py_cFigure_Transform.def(
        pybind11::init<>()
    );

    py_cFigure_Transform.def(
        pybind11::init<double, double, double, double, double, double>(),
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("c"),
        pybind11::arg("d"), pybind11::arg("t1"), pybind11::arg("t2")
    );

    py_cFigure_Transform.def(
        pybind11::self == pybind11::self
    );

    py_cFigure_Transform.def(
        "__str__",
        &omnetpp::cFigure::Transform::str
    );

    py_cFigure_Transform.def(
        "str",
        &omnetpp::cFigure::Transform::str
    );

    py_cFigure_Transform.def(
        "translate",
        pybind11::overload_cast<double, double>(&omnetpp::cFigure::Transform::translate),
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cFigure_Transform.def(
        "translate",
        pybind11::overload_cast<const omnetpp::cFigure::Point &>(&omnetpp::cFigure::Transform::translate),
        pybind11::arg("p")
    );

    py_cFigure_Transform.def(
        "scale",
        pybind11::overload_cast<double>(&omnetpp::cFigure::Transform::scale),
        pybind11::arg("s")
    );

    py_cFigure_Transform.def(
        "scale",
        pybind11::overload_cast<double, double>(&omnetpp::cFigure::Transform::scale),
        pybind11::arg("sx"), pybind11::arg("sy")
    );

    py_cFigure_Transform.def(
        "scale",
        pybind11::overload_cast<double, double, double, double>(&omnetpp::cFigure::Transform::scale),
        pybind11::arg("sx"), pybind11::arg("sy"), pybind11::arg("cx"), pybind11::arg("cy")
    );

    py_cFigure_Transform.def(
        "scale",
        pybind11::overload_cast<double, double, const omnetpp::cFigure::Point &>(&omnetpp::cFigure::Transform::scale),
        pybind11::arg("sx"), pybind11::arg("sy"), pybind11::arg("c")
    );

    py_cFigure_Transform.def(
        "rotate",
        pybind11::overload_cast<double>(&omnetpp::cFigure::Transform::rotate),
        pybind11::arg("phi")
    );

    py_cFigure_Transform.def(
        "rotate",
        pybind11::overload_cast<double, double, double>(&omnetpp::cFigure::Transform::rotate),
        pybind11::arg("phi"), pybind11::arg("cx"), pybind11::arg("cy")
    );

    py_cFigure_Transform.def(
        "rotate",
        pybind11::overload_cast<double, const omnetpp::cFigure::Point &>(&omnetpp::cFigure::Transform::rotate),
        pybind11::arg("phi"), pybind11::arg("c")
    );

    py_cFigure_Transform.def(
        "skewx",
        pybind11::overload_cast<double>(&omnetpp::cFigure::Transform::skewx),
        pybind11::arg("coeff")
    );

    py_cFigure_Transform.def(
        "skewy",
        pybind11::overload_cast<double>(&omnetpp::cFigure::Transform::skewy),
        pybind11::arg("coeff")
    );

    py_cFigure_Transform.def(
        "skewx",
        pybind11::overload_cast<double, double>(&omnetpp::cFigure::Transform::skewy),
        pybind11::arg("coeff"), pybind11::arg("cy")
    );

    py_cFigure_Transform.def(
        "skewy",
        pybind11::overload_cast<double, double>(&omnetpp::cFigure::Transform::skewy),
        pybind11::arg("coeff"), pybind11::arg("cx")
    );

    py_cFigure_Transform.def(
        "multiply",
        &omnetpp::cFigure::Transform::multiply,
        "left-multiply: *this = t * (*this)",
        pybind11::arg("t")
    );

    py_cFigure_Transform.def(
        "rightMultiply",
        &omnetpp::cFigure::Transform::rightMultiply,
        "*this = (*this) * t",
        pybind11::arg("t")
    );

    py_cFigure_Transform.def(
        "applyTo",
        &omnetpp::cFigure::Transform::applyTo,
        pybind11::arg("p")
    );

    pybind11::class_<omnetpp::cFigure::RGBA> py_cFigure_RGBA(
        py_cFigure,
        "RGBA",
        "Components of the pixel."
    );

    py_cFigure_RGBA.def(
        pybind11::init<>()
    );

    py_cFigure_RGBA.def(
        pybind11::init<uint8_t, uint8_t, uint8_t, uint8_t>(),
        pybind11::arg("red"), pybind11::arg("green"), pybind11::arg("blue"), pybind11::arg("alpha")
    );

    py_cFigure_RGBA.def(
        "__str__", &omnetpp::cFigure::RGBA::str
    );

    py_cFigure_RGBA.def(
        "str",
        &omnetpp::cFigure::RGBA::str
    );

    py_cFigure_RGBA.def(
        pybind11::self == pybind11::self
    );

    py_cFigure_RGBA.def(
        "set",
        &omnetpp::cFigure::RGBA::set,
        pybind11::arg("red"), pybind11::arg("green"), pybind11::arg("blue"), pybind11::arg("alpha")
    );

    py_cFigure_RGBA.def("asColor",
        [] (omnetpp::cFigure::RGBA &self)
        {
            return omnetpp::cFigure::Color(self);
        }
    );

    py_cFigure_RGBA.def_readwrite("red", &omnetpp::cFigure::RGBA::red);
    py_cFigure_RGBA.def_readwrite("green", &omnetpp::cFigure::RGBA::green);
    py_cFigure_RGBA.def_readwrite("blue", &omnetpp::cFigure::RGBA::blue);
    py_cFigure_RGBA.def_readwrite("alhpa", &omnetpp::cFigure::RGBA::alpha);


    pybind11::class_<omnetpp::cFigure::Pixmap> py_cFigure_Pixmap(
        py_cFigure,
        "Pixmap",
        "A rectangular RGBA pixel array."
    );

    py_cFigure_Pixmap.def(
        pybind11::init<>()
    );

    py_cFigure_Pixmap.def(
        pybind11::init<int, int>(),
        pybind11::arg("width"), pybind11::arg("height")
    );

    py_cFigure_Pixmap.def(
        pybind11::init<int, int, const omnetpp::cFigure::RGBA &>(),
        pybind11::arg("width"), pybind11::arg("height"), pybind11::arg("fill")
    );

    py_cFigure_Pixmap.def(
        pybind11::init<int, int, const omnetpp::cFigure::Color &, double>(),
        pybind11::arg("width"), pybind11::arg("height"), pybind11::arg("color"), pybind11::arg("opacity")
    );

    py_cFigure_Pixmap.def(
        "setSize",
        pybind11::overload_cast<int, int, const omnetpp::cFigure::RGBA &>(&omnetpp::cFigure::Pixmap::setSize),
        pybind11::arg("width"), pybind11::arg("height"), pybind11::arg("fill")
    );

    py_cFigure_Pixmap.def(
        "setSize",
        pybind11::overload_cast<int, int, const omnetpp::cFigure::Color&, double>(&omnetpp::cFigure::Pixmap::setSize),
        pybind11::arg("width"), pybind11::arg("height"), pybind11::arg("color"), pybind11::arg("opacity")
    );

    py_cFigure_Pixmap.def(
        "fill",
        pybind11::overload_cast<const omnetpp::cFigure::RGBA &>(&omnetpp::cFigure::Pixmap::fill),
        pybind11::arg("fill")
    );

    py_cFigure_Pixmap.def(
        "fill",
        pybind11::overload_cast<const omnetpp::cFigure::Color &, double>(&omnetpp::cFigure::Pixmap::fill),
        pybind11::arg("color"), pybind11::arg("opacity")
    );

    py_cFigure_Pixmap.def(
        "getWidth",
        &omnetpp::cFigure::Pixmap::getWidth
    );

    py_cFigure_Pixmap.def(
        "getHeight",
        &omnetpp::cFigure::Pixmap::getHeight
    );

    py_cFigure_Pixmap.def(
        "pixel",
        pybind11::overload_cast<int, int>(&omnetpp::cFigure::Pixmap::pixel, pybind11::const_),
        pybind11::arg("x"), pybind11::arg("y")
    );

    py_cFigure_Pixmap.def(
        "setPixel",
        &omnetpp::cFigure::Pixmap::setPixel,
        pybind11::arg("x"), pybind11::arg("y"), pybind11::arg("color"), pybind11::arg("opacity")
    );

    py_cFigure_Pixmap.def(
        "getColor",
        &omnetpp::cFigure::Pixmap::getColor,
        pybind11::arg("x"), pybind11::arg("y")
    );

    py_cFigure_Pixmap.def(
        "setColor",
        &omnetpp::cFigure::Pixmap::setColor,
        pybind11::arg("x"), pybind11::arg("y"), pybind11::arg("color")
    );

    py_cFigure_Pixmap.def(
        "getOpacity",
        &omnetpp::cFigure::Pixmap::getOpacity,
        pybind11::arg("x"), pybind11::arg("y")
    );

    py_cFigure_Pixmap.def(
        "setOpacity",
        &omnetpp::cFigure::Pixmap::setOpacity,
        pybind11::arg("x"), pybind11::arg("y"), pybind11::arg("opacity")
    );

    py_cFigure_Pixmap.def(
        "__str__",
        &omnetpp::cFigure::Pixmap::str
    );

    py_cFigure_Pixmap.def(
        "str",
        &omnetpp::cFigure::Pixmap::str
    );


    pybind11::class_<omnetpp::cFigure::Rectangle> py_cFigure_Rectangle(
        py_cFigure,
        "Rectangle",
        "Represents a rectangle as an (x,y,width,height) tuple."
    );

    py_cFigure_Rectangle.def(
        pybind11::init<>()
    );

    py_cFigure_Rectangle.def(
        pybind11::init<double, double, double, double>()
    );

    py_cFigure_Rectangle.def(
        pybind11::self == pybind11::self
    );

    py_cFigure_Rectangle.def(
        "__str__",
        &omnetpp::cFigure::Rectangle::str
    );

    py_cFigure_Rectangle.def(
        "str",
        &omnetpp::cFigure::Rectangle::str
    );

    py_cFigure_Rectangle.def(
        "getCenter",
        &omnetpp::cFigure::Rectangle::getCenter
    );

    py_cFigure_Rectangle.def(
        "getSize",
        &omnetpp::cFigure::Rectangle::getSize
    );

    py_cFigure_Rectangle.def(
        "translate",
        &omnetpp::cFigure::Rectangle::translate,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    pybind11::class_<omnetpp::cFigure::Color> py_cFigure_Color(
        py_cFigure,
        "Color",
        R"docstring(
         Represents an RGB color.

         Conversion from string exists, and accepts HTML colors (\#rrggbb),
         HSB colors in a similar notation (@hhssbb), and English color names
         (SVG color names, to be more precise.) Predefined constants for the
         basic colors (see BLACK, WHITE, GREY, RED, GREEN, BLUE, YELLOW, CYAN,
         MAGENTA), as well as a collection of good dark and light colors, suitable
         for e.g. chart drawing (see GOOD_DARK_COLORS and GOOD_LIGHT_COLORS)
         are provided.
         )docstring"
    );

    py_cFigure_Color.def(pybind11::init<>());

    py_cFigure_Color.def(
        pybind11::init<uint8_t, uint8_t, uint8_t>(),
        pybind11::arg("red"), pybind11::arg("green"), pybind11::arg("blue")
    );

    py_cFigure_Color.def(
        pybind11::init<const char *>(),
        pybind11::arg("color")
    );

    py_cFigure_Color.def(
        "__str__",
        &omnetpp::cFigure::Color::str
    );

    py_cFigure_Color.def(
        "str",
        &omnetpp::cFigure::Color::str
    );

    py_cFigure.def_readonly_static("BLACK", &omnetpp::cFigure::BLACK);
    py_cFigure.def_readonly_static("WHITE", &omnetpp::cFigure::WHITE);
    py_cFigure.def_readonly_static("GREY", &omnetpp::cFigure::GREY);
    py_cFigure.def_readonly_static("RED", &omnetpp::cFigure::RED);
    py_cFigure.def_readonly_static("GREEN", &omnetpp::cFigure::GREEN);
    py_cFigure.def_readonly_static("BLUE", &omnetpp::cFigure::BLUE);
    py_cFigure.def_readonly_static("YELLOW", &omnetpp::cFigure::YELLOW);
    py_cFigure.def_readonly_static("CYAN", &omnetpp::cFigure::CYAN);
    py_cFigure.def_readonly_static("MAGENTA", &omnetpp::cFigure::MAGENTA);
    py_cFigure.def_readonly_static("NUM_GOOD_DARK_COLORS", &omnetpp::cFigure::NUM_GOOD_DARK_COLORS);
    py_cFigure.def_readonly_static("NUM_GOOD_LIGHT_COLORS", &omnetpp::cFigure::NUM_GOOD_LIGHT_COLORS);

    // two functions to expose GOOD_DARK_COLORS and GOOD_LIGHT_COLORS as arrays
    m.def(
        "_getGoodDarkColors",
        [](unsigned int index)
        {
            return omnetpp::cFigure::GOOD_DARK_COLORS[index];
        }
    );

    m.def(
        "_getGoodLightColors",
        [](unsigned int index)
        {
            return omnetpp::cFigure::GOOD_LIGHT_COLORS[index];
        }
    );

    pybind11::class_<omnetpp::cFigure::Font> py_cFigure_Font(
        py_cFigure,
        "Font",
        "Font attributes."
    );

    py_cFigure_Font.def_readwrite(
        "typeface",
        &omnetpp::cFigure::Font::typeface,
        "Typeface of the font. An empty string means the default font."
    );

    py_cFigure_Font.def_readwrite(
        "pointSize",
        &omnetpp::cFigure::Font::pointSize,
        "Font size in points. A zero or negative value means the default size."
    );

    py_cFigure_Font.def_readwrite(
        "style",
        &omnetpp::cFigure::Font::style,
        "Font style. Binary OR of FontStyle constants such as FONT_BOLD."
    );

    py_cFigure_Font.def(pybind11::init<>());

    py_cFigure_Font.def(
        pybind11::init<std::string, int, uint8_t>(),
        pybind11::arg("typeface"), pybind11::arg("pointSize") = -1, pybind11::arg("style") = 0
    );

    py_cFigure_Font.def(
        pybind11::self == pybind11::self
    );

    py_cFigure_Font.def(
        "__str__",
        &omnetpp::cFigure::Font::str
    );

    py_cFigure_Font.def(
        "str",
        &omnetpp::cFigure::Font::str
    );

    pybind11::enum_<omnetpp::cFigure::FontStyle>(
        py_cFigure,
        "FontStyle",
        "Font style constants: FONT_NONE, FONT_BOLD, etc.")
        .value("FONT_NONE", omnetpp::cFigure::FontStyle::FONT_NONE)
        .value("FONT_BOLD", omnetpp::cFigure::FontStyle::FONT_BOLD)
        .value("FONT_ITALIC", omnetpp::cFigure::FontStyle::FONT_ITALIC)
        .value("FONT_UNDERLINE", omnetpp::cFigure::FontStyle::FONT_UNDERLINE)
        .export_values();

    pybind11::enum_<omnetpp::cFigure::LineStyle>(
        py_cFigure,
        "LineStyle",
        "Line style constants: LINE_SOLID, LINE_DOTTED, etc.")
        .value("LINE_SOLID", omnetpp::cFigure::LineStyle::LINE_SOLID)
        .value("LINE_DOTTED", omnetpp::cFigure::LineStyle::LINE_DOTTED)
        .value("LINE_DASHED", omnetpp::cFigure::LineStyle::LINE_DASHED)
        .export_values();

    pybind11::enum_<omnetpp::cFigure::CapStyle>(
        py_cFigure,
        "CapStyle",
        "Line cap style constants: CAP_BUTT, CAP_SQUARE, etc.")
        .value("CAP_BUTT", omnetpp::cFigure::CapStyle::CAP_BUTT)
        .value("CAP_SQUARE", omnetpp::cFigure::CapStyle::CAP_SQUARE)
        .value("CAP_ROUND", omnetpp::cFigure::CapStyle::CAP_ROUND)
        .export_values();

    pybind11::enum_<omnetpp::cFigure::JoinStyle>(
        py_cFigure,
        "JoinStyle",
        "Line join style constants: JOIN_BEVEL, JOIN_MITER, etc.")
        .value("JOIN_BEVEL", omnetpp::cFigure::JoinStyle::JOIN_BEVEL)
        .value("JOIN_MITER", omnetpp::cFigure::JoinStyle::JOIN_MITER)
        .value("JOIN_ROUND", omnetpp::cFigure::JoinStyle::JOIN_ROUND)
        .export_values();

    pybind11::enum_<omnetpp::cFigure::FillRule>(
        py_cFigure,
        "FillRule",
        "Fill rule constants: FILL_EVENODD, FILL_NONZERO.")
        .value("FILL_EVENODD", omnetpp::cFigure::FillRule::FILL_EVENODD)
        .value("FILL_NONZERO", omnetpp::cFigure::FillRule::FILL_NONZERO)
        .export_values();

    pybind11::enum_<omnetpp::cFigure::Arrowhead>(
        py_cFigure,
        "Arrowhead",
        "Arrowhead style constants: ARROW_NONE, ARROW_SIMPLE, etc.")
        .value("ARROW_NONE", omnetpp::cFigure::Arrowhead::ARROW_NONE)
        .value("ARROW_SIMPLE", omnetpp::cFigure::Arrowhead::ARROW_SIMPLE)
        .value("ARROW_TRIANGLE", omnetpp::cFigure::Arrowhead::ARROW_TRIANGLE)
        .value("ARROW_BARBED", omnetpp::cFigure::Arrowhead::ARROW_BARBED)
        .export_values();

    pybind11::enum_<omnetpp::cFigure::Interpolation>(
        py_cFigure,
        "Interpolation",
        "Image interpolation mode constants: INTERPOLATION_NONE, INTERPOLATION_FAST, etc.")
        .value("INTERPOLATION_NONE", omnetpp::cFigure::Interpolation::INTERPOLATION_NONE)
        .value("INTERPOLATION_FAST", omnetpp::cFigure::Interpolation::INTERPOLATION_FAST)
        .value("INTERPOLATION_BEST", omnetpp::cFigure::Interpolation::INTERPOLATION_BEST)
        .export_values();

    pybind11::enum_<omnetpp::cFigure::Anchor>(
        py_cFigure,
        "Anchor",
        "Anchoring mode constants: ANCHOR_CENTER, ANCHOR_N, etc.")
        .value("ANCHOR_CENTER", omnetpp::cFigure::Anchor::ANCHOR_CENTER)
        .value("ANCHOR_N", omnetpp::cFigure::Anchor::ANCHOR_N)
        .value("ANCHOR_E", omnetpp::cFigure::Anchor::ANCHOR_E)
        .value("ANCHOR_S", omnetpp::cFigure::Anchor::ANCHOR_S)
        .value("ANCHOR_W", omnetpp::cFigure::Anchor::ANCHOR_W)
        .value("ANCHOR_NW", omnetpp::cFigure::Anchor::ANCHOR_NW)
        .value("ANCHOR_NE", omnetpp::cFigure::Anchor::ANCHOR_NE)
        .value("ANCHOR_SE", omnetpp::cFigure::Anchor::ANCHOR_SE)
        .value("ANCHOR_SW", omnetpp::cFigure::Anchor::ANCHOR_SW)
        .value("ANCHOR_BASELINE_START", omnetpp::cFigure::Anchor::ANCHOR_BASELINE_START)
        .value("ANCHOR_BASELINE_MIDDLE", omnetpp::cFigure::Anchor::ANCHOR_BASELINE_MIDDLE)
        .value("ANCHOR_BASELINE_END", omnetpp::cFigure::Anchor::ANCHOR_BASELINE_END)
        .export_values();

    py_cFigure.def_static(
        "parsePoint",
        pybind11::overload_cast<const char *>(&omnetpp::cFigure::parsePoint),
        "parse Point::str() format",
        pybind11::arg("s")
    );

    py_cFigure.def_static(
        "parseRectangle",
        &omnetpp::cFigure::parseRectangle,
        "parse Rectangle::str() format",
        pybind11::arg("s")
    );

    py_cFigure.def_static(
        "parseTransform",
        pybind11::overload_cast<const char *>(&omnetpp::cFigure::parseTransform),
        "parse Transform::str() format",
        pybind11::arg("s")
    );

    py_cFigure.def_static(
        "parseFont",
        pybind11::overload_cast<const char *>(&omnetpp::cFigure::parseFont),
        "parse Font::str() format",
        pybind11::arg("s")
    );

    py_cFigure.def_static(
        "parseColor",
        &omnetpp::cFigure::parseColor,
        "parse Color::str() format",
        pybind11::arg("s")
    );

    py_cFigure.def_static(
        "parseBool",
        &omnetpp::cFigure::parseBool,
        "parse Bool::str() format",
        pybind11::arg("s")
    );

    py_cFigure.def_static(
        "parseLineStyle",
        &omnetpp::cFigure::parseLineStyle,
        "parse LineStyle::str() format",
        pybind11::arg("s")
    );

    py_cFigure.def_static(
        "parseCapStyle",
        &omnetpp::cFigure::parseCapStyle,
        "parse CapStyle::str() format",
        pybind11::arg("s")
    );

    py_cFigure.def_static(
        "parseJoinStyle",
        &omnetpp::cFigure::parseJoinStyle,
        "parse JoinStyle::str() format",
        pybind11::arg("s")
    );

    py_cFigure.def_static(
        "parseFillRule",
        &omnetpp::cFigure::parseFillRule,
        "parse FillRule::str() format",
        pybind11::arg("s")
    );

    py_cFigure.def_static(
        "parseArrowhead",
        &omnetpp::cFigure::parseArrowhead,
        "parse Arrowhead::str() format",
        pybind11::arg("s")
    );

    py_cFigure.def_static(
        "parseInterpolation",
        &omnetpp::cFigure::parseInterpolation,
        "parse Interpolation::str() format",
        pybind11::arg("s")
    );

    py_cFigure.def_static(
        "parseAnchor",
        &omnetpp::cFigure::parseAnchor,
        "parse Anchor::str() format",
        pybind11::arg("s")
    );

    py_cFigure.def(
        "dup",
        &omnetpp::cFigure::dup,
        R"docstring(
        Clones the figure, ignoring its child figures. (The copy will have
        no children.) To clone a figure together with its figure subtree,
        use dupTree().
        )docstring"
    );

    py_cFigure.def(
        "forEachChild",
        &omnetpp::cFigure::forEachChild,
        R"docstring(
         Calls v.visit(self) on each child figure.
         See cObject for more details.
        )docstring",
        pybind11::arg("v")
    );

    py_cFigure.def(
        "str",
        &omnetpp::cFigure::str,
        "Returns a one-line string with the most characteristic data of the figure."
    );

    py_cFigure.def(
        "__str__",
        &omnetpp::cFigure::str,
        "Returns a one-line string with the most characteristic data of the figure."
    );

    py_cFigure.def(
        "getId",
        &omnetpp::cFigure::getId,
        R"docstring(
        Returns the unique ID of this figure object. This ID is mostly
        intended for internal use.
        )docstring"
    );

    py_cFigure.def(
        "isVisible",
        &omnetpp::cFigure::isVisible,
        R"docstring(
        Returns the visibility flag of the figure.
        )docstring"
    );

    py_cFigure.def(
        "setVisible",
        &omnetpp::cFigure::setVisible,
        R"docstring(
        Sets the visibility flag of the figure. Setting the visibility flag
        to false will cause the whole figure subtree to be omitted from
        rendering.
        )docstring",
        pybind11::arg("visible")
    );

    py_cFigure.def(
        "getTransform",
        &omnetpp::cFigure::getTransform,
        R"docstring(
        Returns the transform associated with this figure.
        )docstring"
    );

    py_cFigure.def(
        "setTransform",
        &omnetpp::cFigure::setTransform,
        R"docstring(
        Sets the transform associated with this figure. The transform will
        affect the rendering of this figure and its whole figure subtree.
        The transform may also be modified with the various
        translate(), scale(), rotate(), skewx()/skewy() methods.
        )docstring",
        pybind11::arg("transform")
    );

    py_cFigure.def(
        "resetTransform",
        &omnetpp::cFigure::resetTransform,
        R"docstring(
        Convenience method: sets the transformation to identity transform.
        )docstring"
    );

    py_cFigure.def(
        "getZIndex",
        &omnetpp::cFigure::getZIndex,
        "Returns the Z-index of the figure."
    );

    py_cFigure.def(
        "setZIndex",
        &omnetpp::cFigure::setZIndex,
        R"docstring(
        Sets the Z-index of the figure. The Z-index affects the stacking
        order of figures via the effective Z-index. The effective Z-index
        is computed as the sum of the Z-indices of this figure and its
        ancestors'. A figure with a greater effective Z-index is always in
        front of an element with a lower effective Z-index. For figures
        with the same effective Z-index, their relative order in the
        preorder traversal of the figure hierarchy defines the stacking
        order. (A later figure is in front of the earlier ones.)
        )docstring",
        pybind11::arg("zIndex")
    );

    py_cFigure.def(
        "getEffectiveZIndex",
        &omnetpp::cFigure::getEffectiveZIndex,
        R"docstring(
        Returns the effective Z-index of the figure. The effective Z-index
        is computed as the sum of the Z-indices of this figure and its
        ancestors'. A figure with a greater effective Z-index is always in
        front of an element with a lower effective Z-index.
        )docstring"
    );

    py_cFigure.def(
        "getTooltip",
        &omnetpp::cFigure::getTooltip,
        R"docstring(
        Returns the tooltip of the figure, or `None` if it does not have one.
        )docstring"
    );

    py_cFigure.def(
        "setTooltip",
        &omnetpp::cFigure::setTooltip,
        R"docstring(
        Sets the tooltip of the figure. Pass `None` to clear the tooltip.
        If `None` is set, the effective tooltip will be inherited
        from the associated object if there is one (see getAssociatedObject()),
        or from the parent figure. Empty tooltips will not be shown.
        )docstring",
        pybind11::arg("tooltip")
    );

    py_cFigure.def(
        "getAssociatedObject",
        &omnetpp::cFigure::getAssociatedObject,
        R"docstring(
        Returns the simulation object associated with this figure, or `None`
        if it does not have one. The GUI may use this function to provide a
        shortcut access to the specified object, e.g. select the object in an
        inspector when the user clicks the figure.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cFigure.def(
        "setAssociatedObject",
        &omnetpp::cFigure::setAssociatedObject,
        R"docstring(
        Sets the simulation object associated with this figure. The user is
        responsible for ensuring that the pointer is valid (the corresponding
        object exists), and remains valid while it is associated with the figure.
        )docstring"
    );

    py_cFigure.def(
        "getTags",
        &omnetpp::cFigure::getTags,
        R"docstring(
        Returns the space-separated list of the tags associated with the figure.
        Tags may be used in the GUI displaying a canvas for implementing
        layers or other kind of filtering.
        )docstring"
    );

    py_cFigure.def(
        "setTags",
        &omnetpp::cFigure::setTags,
        R"docstring(
        Sets the list of tags associated with the figure. This method accepts
        a space-separated list of tags. (Tags themselves may not contain spaces;
        underscores are recommended instead where needed.)
        )docstring",
        pybind11::arg("tags")
    );

    py_cFigure.def(
        "getParentFigure",
        &omnetpp::cFigure::getParentFigure,
        R"docstring(
        Returns the parent figure of this figure, or nullptr if it has none.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cFigure.def(
        "getCanvas",
        &omnetpp::cFigure::getCanvas,
        R"docstring(
        Returns the canvas object to which this figure was added, or `None`
        if there is no such canvas. Note that the runtime cost of this method
        is proportional to the depth of this figure in the figure tree.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cFigure.def(
        "getNumFigures",
        &omnetpp::cFigure::getNumFigures,
        R"docstring(
        Returns the number of child figures.
        )docstring"
    );

    py_cFigure.def(
        "getFigure",
        pybind11::overload_cast<int>(&omnetpp::cFigure::getFigure, pybind11::const_),
        R"docstring(
        Returns the kth figure in the child list. The index must be in the
        range 0..getNumFigures()-1. An out-of-bounds index will cause a runtime
        error.
        )docstring",
        pybind11::arg("pos"),
        pybind11::return_value_policy::reference
    );

    py_cFigure.def(
        "getFigure",
        pybind11::overload_cast<const char *>(&omnetpp::cFigure::getFigure, pybind11::const_),
        R"docstring(
        Returns the kth figure in the child list. The index must be in the
        range 0..getNumFigures()-1. An out-of-bounds index will cause a runtime
        error.
        )docstring",
        pybind11::arg("name"),
        pybind11::return_value_policy::reference
    );

    py_cFigure.def(
        "findFigure",
        pybind11::overload_cast<const char *>(&omnetpp::cFigure::findFigure, pybind11::const_),
        R"docstring(
        Finds the given figure among the children of this figure, and returns
        its index. If it is not found, -1 is returned.
        )docstring",
        pybind11::arg("name")
    );

    py_cFigure.def(
        "findFigure",
        pybind11::overload_cast<const omnetpp::cFigure *>(&omnetpp::cFigure::findFigure, pybind11::const_),
        R"docstring(
        Finds the given figure among the children of this figure, and returns
        its index. If it is not found, -1 is returned.
        )docstring",
        pybind11::arg("name")
    );

    py_cFigure.def(
        "containsFigures",
        &omnetpp::cFigure::containsFigures,
        R"docstring(
        Returns true if this figure has child figures, and false otherwise.
        )docstring"
    );

    py_cFigure.def(
        "findFigureRecursively",
        &omnetpp::cFigure::findFigureRecursively,
        R"docstring(
        Find the first figure with the given name in this figure's subtree,
        including the figure itself. Returns `None` if no such figure is found.
        )docstring",
        pybind11::arg("name"),
        pybind11::return_value_policy::reference
    );

    py_cFigure.def(
        "getFigureByPath",
        &omnetpp::cFigure::getFigureByPath,
        R"docstring(
        Finds a figure in the subtree, given by its absolute or relative path.
        The path is a string of figure names separated by dots; the special
        module name ^ (caret) stands for the parent figure. If the path starts
        with a dot or caret, it is understood as relative to this figure,
        otherwise it is taken to mean an absolute path (i.e. relative to the
        root figure, see getRootFigure()). Returns `None` if the figure
        was not found.

        Examples:
          - "." means this figure;
          - ".icon" means the child figure name "icon";
          - ".group.label" means the "label" child of the "group" child figure;
          - "^.icon" or ".^.icon" means the "icon" sibling of this figure;
          - "icon" means the "icon" child of the root figure

        The above syntax is similar to the one used by cModule::getModuleByPath()
        )docstring",
        pybind11::arg("name"),
        pybind11::return_value_policy::reference
    );

    py_cFigure.def(
        "addFigure",
        pybind11::overload_cast<omnetpp::cFigure *>(&omnetpp::cFigure::addFigure),
        R"docstring(
        Appends the given figure to the child list of this figure.
        )docstring",
        pybind11::arg("figure")
    );

    py_cFigure.def(
        "addFigure",
        pybind11::overload_cast<omnetpp::cFigure *, int>(&omnetpp::cFigure::addFigure),
        R"docstring(
        Inserts the given figure into the child list of this figure at
        the given position. Note that relative order in the child list
        only affects stacking order if the respective figures have the
        same Z-index.
        )docstring",
        pybind11::arg("figure"), pybind11::arg("pos")
    );

    py_cFigure.def(
        "removeFigure",
        pybind11::overload_cast<omnetpp::cFigure *>(&omnetpp::cFigure::removeFigure),
        R"docstring(
        Removes the given figure from the child list of this figure.
        An error is raised if the figure is not a child of this figure.

        Prefer removeFromParent() to this method.
        )docstring",
        pybind11::arg("figure"),
        pybind11::return_value_policy::reference
    );

    py_cFigure.def(
        "removeFigure",
        pybind11::overload_cast<int>(&omnetpp::cFigure::removeFigure),
        R"docstring(
        Removes the kth figure from the child list of this figure. An error
        is raised if pos is not in the range 0..getNumFigures()-1.

        Prefer removeFromParent() to this method.
        )docstring",
        pybind11::arg("figure"),
        pybind11::return_value_policy::reference
    );

    py_cFigure.def(
        "removeFromParent",
        &omnetpp::cFigure::removeFromParent,
        R"docstring(
        Removes this figure from the child list of its parent figure.
        It has no effect if the figure has no parent figure.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cFigure.def(
        "isAbove",
        &omnetpp::cFigure::isAbove,
        R"docstring(
        Returns true if this figure is above the given reference figure
        in stacking order, based on their Z-index values and their relative
        order in the parent's child list, and false if not. The method
        throws an error if the two figures are not under the same parent.
        )docstring",
        pybind11::arg("figure")
    );

    py_cFigure.def(
        "isBelow",
        &omnetpp::cFigure::isBelow,
        R"docstring(
        Returns true if this figure is below the given reference figure
        in stacking order, based on their Z-index values and their relative
        order in the parent's child list, and false if not. The method
        throws an error if the two figures are not under the same parent.
        )docstring",
        pybind11::arg("figure")
    );

    py_cFigure.def(
        "insertAbove",
        &omnetpp::cFigure::insertAbove,
        R"docstring(
        Inserts this figure under the same parent as the reference figure,
        guaranteeing that it will appear above the reference figure
        in stacking order. In order to achieve the latter, the Z-index
        of the figure is set to the Z-index of the reference figure if
        it was smaller. In case of equal Z-indices, the figure is inserted
        right after the reference figure. If the Z-index of the figure is
        already greater than that of the reference figure, nothing should be
        assumed about the insertion position.
        )docstring",
        pybind11::arg("referenceFigure")
    );

    py_cFigure.def(
        "insertBelow",
        &omnetpp::cFigure::insertBelow,
        R"docstring(
        Inserts this figure under the same parent as the reference figure,
        guaranteeing that it will appear below the reference figure
        in stacking order. In order to achieve the latter, the Z-index
        of the figure is set to the Z-index of the reference figure if
        it was greater. In case of equal Z-indices, the figure is inserted
        right before the reference figure. If the Z-index of the figure is
        already smaller than that of the reference figure, nothing should be
        assumed about the insertion position.
        )docstring",
        pybind11::arg("referenceFigure")
    );

    py_cFigure.def(
        "insertAfter",
        &omnetpp::cFigure::insertAfter,
        R"docstring(
        Inserts this figure after the given reference figure in its parent's
        child list. BEWARE: Relative order in the child list only affects
        stacking order if the respective figures have the same Z-index.
        )docstring",
        pybind11::arg("referenceFigure")
    );

    py_cFigure.def(
        "insertBefore",
        &omnetpp::cFigure::insertBefore,
        R"docstring(
        Inserts this figure in front of the given reference figure in its
        parent's child list. BEWARE: Relative order in the child list only
        affects stacking order if the respective figures have the same Z-index.
        )docstring",
        pybind11::arg("referenceFigure")
    );

    py_cFigure.def(
        "raiseAbove",
        &omnetpp::cFigure::raiseAbove,
        R"docstring(
        If this figure is below the given reference figure in stacking order,
        move it directly above. This is achieved by updating its Z-index
        if needed, and moving it in the parent's child list. An error is
        raised if the two figures do not share the same parent.
        )docstring",
        pybind11::arg("referenceFigure")
    );

    py_cFigure.def(
        "lowerBelow",
        &omnetpp::cFigure::lowerBelow,
        R"docstring(
        If this figure is above the given reference figure in stacking order,
        move it directly below. This is achieved by updating its Z-index
        if needed, and moving it in the parent's child list. An error is
        raised if the two figures do not share the same parent.
        )docstring",
        pybind11::arg("referenceFigure")
    );

    py_cFigure.def(
        "raiseToTop",
        &omnetpp::cFigure::raiseToTop,
        R"docstring(
        Lower this figure below all other figures under the same parent. This is
        achieved by updating its Z-index if needed (setting it to be at most
        the minimum of the Z-indices of the other figures), and moving it in the
        parent's child list. It is an error if this figure has no parent.
        )docstring"
    );

    py_cFigure.def(
        "lowerToBottom",
        &omnetpp::cFigure::lowerToBottom,
        R"docstring(
        Lower this figure below all other figures under the same parent. This is
        achieved by updating its Z-index if needed (setting it to be at most
        the minimum of the Z-indices of the other figures), and moving it in the
        parent's child list. It is an error if this figure has no parent.
        )docstring"
    );

    py_cFigure.def(
        "dupTree",
        &omnetpp::cFigure::dup,
        R"docstring(
        Duplicate the figure subtree.
        )docstring"
    );

    py_cFigure.def(
        "translate",
        &omnetpp::cFigure::translate,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cFigure.def(
        "scale",
        pybind11::overload_cast<double>(&omnetpp::cFigure::scale),
        pybind11::arg("s")
    );

    py_cFigure.def(
        "scale",
        pybind11::overload_cast<double, double>(&omnetpp::cFigure::scale),
        pybind11::arg("sx"), pybind11::arg("sy")
    );

    py_cFigure.def(
        "scale",
        pybind11::overload_cast<double, double, double, double>(&omnetpp::cFigure::scale),
        pybind11::arg("sx"), pybind11::arg("sy"), pybind11::arg("cx"), pybind11::arg("cy")
    );

    py_cFigure.def(
        "scale",
        pybind11::overload_cast<double, double, const omnetpp::cFigure::Point &>(&omnetpp::cFigure::scale),
        pybind11::arg("sx"), pybind11::arg("sy"), pybind11::arg("c")
    );

    py_cFigure.def(
        "rotate",
        pybind11::overload_cast<double>(&omnetpp::cFigure::rotate),
        pybind11::arg("phi")
    );

    py_cFigure.def(
        "rotate",
        pybind11::overload_cast<double, double, double>(&omnetpp::cFigure::rotate),
        pybind11::arg("phi"), pybind11::arg("cx"), pybind11::arg("cy")
    );

    py_cFigure.def(
        "rotate",
        pybind11::overload_cast<double, const omnetpp::cFigure::Point &>(&omnetpp::cFigure::rotate),
        pybind11::arg("phi"), pybind11::arg("c")
    );

    py_cFigure.def(
        "skewx",
        pybind11::overload_cast<double>(&omnetpp::cFigure::skewx),
        pybind11::arg("coeff")
    );

    py_cFigure.def(
        "skewx",
        pybind11::overload_cast<double, double>(&omnetpp::cFigure::skewx),
        pybind11::arg("coeff"), pybind11::arg("cx")
    );

    py_cFigure.def(
        "skewy",
        pybind11::overload_cast<double>(&omnetpp::cFigure::skewy),
        pybind11::arg("coeff")
    );

    py_cFigure.def(
        "skewy",
        pybind11::overload_cast<double, double>(&omnetpp::cFigure::skewy),
        pybind11::arg("coeff"), pybind11::arg("cy")
    );

    py_cFigure.def(
        "moveLocal",
        &omnetpp::cFigure::moveLocal,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cFigure.def("asLineFigure", [] (omnetpp::cFigure &self) {return omnetpp::check_and_cast<omnetpp::cLineFigure*>(&self);});
    py_cFigure.def("asArcFigure", [] (omnetpp::cFigure &self) {return omnetpp::check_and_cast<omnetpp::cArcFigure*>(&self);});
    py_cFigure.def("asPolylineFigure", [] (omnetpp::cFigure &self) {return omnetpp::check_and_cast<omnetpp::cPolylineFigure*>(&self);});

    py_cFigure.def("asRectangleFigure", [] (omnetpp::cFigure &self) {return omnetpp::check_and_cast<omnetpp::cRectangleFigure*>(&self);});
    py_cFigure.def("asOvalFigure", [] (omnetpp::cFigure &self) {return omnetpp::check_and_cast<omnetpp::cOvalFigure*>(&self);});
    py_cFigure.def("asRingFigure", [] (omnetpp::cFigure &self) {return omnetpp::check_and_cast<omnetpp::cRingFigure*>(&self);});
    py_cFigure.def("asPieSliceFigure", [] (omnetpp::cFigure &self) {return omnetpp::check_and_cast<omnetpp::cPieSliceFigure*>(&self);});
    py_cFigure.def("asPolygonFigure", [] (omnetpp::cFigure &self) {return omnetpp::check_and_cast<omnetpp::cPolygonFigure*>(&self);});
    py_cFigure.def("asPathFigure", [] (omnetpp::cFigure &self) {return omnetpp::check_and_cast<omnetpp::cPathFigure*>(&self);});

    py_cFigure.def("asTextFigure", [] (omnetpp::cFigure &self) {return omnetpp::check_and_cast<omnetpp::cTextFigure*>(&self);});
    py_cFigure.def("asLabelFigure", [] (omnetpp::cFigure &self) {return omnetpp::check_and_cast<omnetpp::cLabelFigure*>(&self);});
    py_cFigure.def("asImageFigure", [] (omnetpp::cFigure &self) {return omnetpp::check_and_cast<omnetpp::cImageFigure*>(&self);});
    py_cFigure.def("asIconFigure", [] (omnetpp::cFigure &self) {return omnetpp::check_and_cast<omnetpp::cIconFigure*>(&self);});
    py_cFigure.def("asPixmapFigure", [] (omnetpp::cFigure &self) {return omnetpp::check_and_cast<omnetpp::cPixmapFigure*>(&self);});

}

void bind_cGroupFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cGroupFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cGroupFigure, pybind11::nodelete>
        > py_cGroupFigure(
        m,
        "_cGroupFigure",
        R"docstring(
        A figure with the sole purpose of grouping its children, and no visual
        representation.

        The usefulness of a group figure comes from the fact that transformations
        are inherited from parent to child, thus, children of a group can be moved,
        scaled, rotated, etc. together by updating the group's transformation matrix.
        )docstring"
    );

    py_cGroupFigure.def(
        pybind11::init<const char *>(),
        pybind11::arg("name") = nullptr
    );

    py_cGroupFigure.def(
        "dup",
        &omnetpp::cGroupFigure::dup
    );

    py_cGroupFigure.def(
        "str",
        &omnetpp::cGroupFigure::str
    );

    py_cGroupFigure.def(
        "moveLocal",
        &omnetpp::cGroupFigure::moveLocal,
        pybind11::arg("dx"), pybind11::arg("dy")
    );
}

void bind_cPanelFigure(pybind11::module &m)
{

    pybind11::class_<
        omnetpp::cPanelFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cPanelFigure, pybind11::nodelete>
        > py_cPanelFigure(
        m,
        "_cPanelFigure",
        R"docstring(

        Sets up an axis-aligned, unscaled coordinate system for children, canceling the
        effect of any transformation (scaling, rotation, etc.) inherited from ancestor figures.

        This allows pixel-based positioning of children, and makes them immune to zooming.
        The anchorPoint in the coordinate system is mapped to the position of the panel figure.
        Setting a transformation on the panel figure itself allows rotation, scaling,
        and skewing of the coordinate system. The anchorPoint is affected by this transformation.

        The panel figure itself has no visual appearance.
        */)docstring"
    );

    py_cPanelFigure.def(
        pybind11::init<const char *>(),
        pybind11::arg("name") = nullptr
    );

    py_cPanelFigure.def(
        "dup",
        &omnetpp::cPanelFigure::dup
    );

    py_cPanelFigure.def(
        "__str__",
        &omnetpp::cPanelFigure::str
    );

    py_cPanelFigure.def(
        "str",
        &omnetpp::cPanelFigure::str
    );

    py_cPanelFigure.def(
        "updateParentTransform",
        &omnetpp::cPanelFigure::updateParentTransform,
        pybind11::arg("transform")
    );

    py_cPanelFigure.def(
        "move",
        &omnetpp::cPanelFigure::move,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cPanelFigure.def(
        "moveLocal",
        &omnetpp::cPanelFigure::moveLocal,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cPanelFigure.def(
        "getPosition",
        &omnetpp::cPanelFigure::getPosition
    );

    py_cPanelFigure.def(
        "setPosition",
        &omnetpp::cPanelFigure::setPosition,
        pybind11::arg("position")
    );

    py_cPanelFigure.def(
        "getAnchor",
        &omnetpp::cPanelFigure::getAnchorPoint,
        R"docstring(
        By default, the (0,0) point in cPanelFigure's coordinate system will be mapped
        to the given position (i.e. getPosition()) in the parent figure's coordinate system.
        By setting an anchorPoint, one can change (0,0) to an arbitrary point. E.g. by setting
        anchorPoint=(100,0), the (100,0) point will be mapped to the given position, i.e.
        panel contents will appear 100 pixels to the left (given there are no transforms set).
        The translation part of the local transform is cancelled out because the anchorPoint
        is subject to the transformation of the panel figure the same way as the child figures.
        )docstring"
    );

    py_cPanelFigure.def(
        "setAnchorPoint",
        &omnetpp::cPanelFigure::setAnchorPoint
    );
}

void bind_cAbstractLineFigure(pybind11::module &m)
{

    pybind11::class_<omnetpp::cAbstractLineFigure, omnetpp::cFigure> py_cAbstractLineFigure(
        m,
        "_cAbstractLineFigure",
        R"docstring(
        Common base class for line figures.

        This class provides line color, style, width, opacity, and other properties.
        Lines may also be augmented with arrowheads at either or both ends.

        Transformations such as scaling or skew do affect the width of the line as it
        is rendered on the canvas. Whether zooming (by the user) should also affect
        it can be controlled by setting a flag (see setZoomLineWidth()).

        The rendering of zero-width lines is currently undefined. It is attempted
        to be rendered as a one pixel wide line, regardless of transforms and zoom
        level, but it is not possible on all platform.
        )docstring"
    );

    // commented out as it is abstract
    // py_cAbstractLineFigure.def( pybind11::init<const char *>(), pybind11::arg("name") = nullptr);

    py_cAbstractLineFigure.def(
        "dup",
        &omnetpp::cAbstractLineFigure::dup
    );

    py_cAbstractLineFigure.def(
        "__str__",
        &omnetpp::cAbstractLineFigure::str
    );

    py_cAbstractLineFigure.def(
        "str",
        &omnetpp::cAbstractLineFigure::str
    );

    py_cAbstractLineFigure.def(
        "parse",
        &omnetpp::cAbstractLineFigure::parse
    );

    py_cAbstractLineFigure.def(
        "getLineColor",
        &omnetpp::cAbstractLineFigure::getLineColor,
        pybind11::return_value_policy::reference
    );

    py_cAbstractLineFigure.def(
        "setLineColor",
        &omnetpp::cAbstractLineFigure::setLineColor,
        pybind11::arg("lineColor")
    );

    py_cAbstractLineFigure.def(
        "getLineWidth",
        &omnetpp::cAbstractLineFigure::getLineWidth,
        R"docstring(
        Returns the line width of the figure. Note that the actual line
        width will also be affected by the figure transformations and,
        if zoomLineWidth is set, by the zoom level as well.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractLineFigure.def(
        "setLineWidth",
        &omnetpp::cAbstractLineFigure::setLineWidth,
        R"docstring(
        Sets the line width of the figure. The number must be
        positive (zero is not allowed). The default line width is 1.
        See also setZoomLineWidth().
        )docstring"
    );

    py_cAbstractLineFigure.def(
        "getLineOpacity",
        &omnetpp::cAbstractLineFigure::getLineOpacity,
        R"docstring(
        Returns the line opacity of the figure, a number in the [0,1] interval.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractLineFigure.def(
        "setLineOpacity",
        &omnetpp::cAbstractLineFigure::setLineOpacity,
        R"docstring(
        Sets the line opacity of the figure. The argument must be a number
        in the [0,1] interval. The default opacity is 1.0.
        )docstring",
        pybind11::arg("lineOpacity")
    );

    py_cAbstractLineFigure.def(
        "getLineStyle",
        &omnetpp::cAbstractLineFigure::getLineStyle,
        R"docstring(
        Returns the line style of the figure.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractLineFigure.def(
        "setLineStyle",
        &omnetpp::cAbstractLineFigure::setLineStyle,
        R"docstring(
        Sets the line style of the figure (dotted, dashed, etc.)
        The default line style is solid.
        )docstring",
        pybind11::arg("lineStyle")
    );

    py_cAbstractLineFigure.def(
        "getCapStyle",
        &omnetpp::cAbstractLineFigure::getCapStyle,
        R"docstring(
        Returns the cap style for the figure's line.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractLineFigure.def(
        "setCapStyle",
        &omnetpp::cAbstractLineFigure::setCapStyle,
        R"docstring(
        Sets the cap style for the figure's line. The default cap style is butt.
        )docstring",
        pybind11::arg("capStyle")
    );

    py_cAbstractLineFigure.def(
        "getStartArrowhead",
        &omnetpp::cAbstractLineFigure::getStartArrowhead,
        R"docstring(
        Returns the type of the arrowhead at the start of the line.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractLineFigure.def(
        "setStartArrowhead",
        &omnetpp::cAbstractLineFigure::setStartArrowhead,
        R"docstring(
        Sets the type of the arrowhead at the start of the line.
        The default is no arrowhead.
        )docstring",
        pybind11::arg("startArrowhead")
    );

    py_cAbstractLineFigure.def(
        "getEndArrowhead",
        &omnetpp::cAbstractLineFigure::getEndArrowhead,
        R"docstring(
        Returns the type of the arrowhead at the end of the line.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractLineFigure.def(
        "setZoomLineWidth",
        &omnetpp::cAbstractLineFigure::setZoomLineWidth,
        R"docstring(
        Sets the flag that controls whether zoom level should affect
        the line width the figure is drawn with. The default setting
        is false, i.e. zoom does not affect line width.
        )docstring",
        pybind11::arg("zoomLineWidth")
    );

    py_cAbstractLineFigure.def(
        "getZoomLineWidth",
        &omnetpp::cAbstractLineFigure::getZoomLineWidth,
        R"docstring(
        Returns true if zoom level affects the line width the figure is
        drawn with, and false if it does not.
        )docstring"
    );
}

void bind_cLineFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cLineFigure, omnetpp::cAbstractLineFigure,  omnetpp::cFigure,
        std::unique_ptr<omnetpp::cLineFigure, pybind11::nodelete>
        > py_cLineFigure(
        m,
        "_cLineFigure",
        R"docstring(
        A figure that displays a straight line segment.

        This class provides the coordinates of the end points. Other properties
        such as color and line style are inherited from cAbstractLineFigure.
        )docstring"
    );

    py_cLineFigure.def(
        pybind11::init<const char *>(),
        pybind11::arg("name") = nullptr
    );

    py_cLineFigure.def(
        "dup",
        &omnetpp::cLineFigure::dup
    );

    py_cLineFigure.def(
        "__str__",
        &omnetpp::cLineFigure::str
    );

    py_cLineFigure.def(
        "str",
        &omnetpp::cLineFigure::str
    );

    py_cLineFigure.def(
        "parse",
        &omnetpp::cLineFigure::parse
    );

    py_cLineFigure.def(
        "moveLocal",
        &omnetpp::cLineFigure::moveLocal,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cLineFigure.def(
        "getStart",
        &omnetpp::cLineFigure::getStart,
        R"docstring(
        Returns the start point of the line segment.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cLineFigure.def(
        "setStart",
        &omnetpp::cLineFigure::setStart,
        R"docstring(
        Sets the start point of the line segment.
        )docstring",
        pybind11::arg("start")
    );

    py_cLineFigure.def(
        "getEnd",
        &omnetpp::cLineFigure::getEnd,
        R"docstring(
        Returns the end point of the line segment.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cLineFigure.def(
        "setEnd",
        &omnetpp::cLineFigure::setEnd,
        R"docstring(
        Sets the end point of the line segment.
        )docstring",
        pybind11::arg("end")
    );
}

void bind_cArcFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cArcFigure, omnetpp::cAbstractLineFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cArcFigure, pybind11::nodelete>
        > py_cArcFigure(
        m,
        "_cArcFigure",
        R"docstring(
        A figure that displays an arc.

        An arc's geometry is determined by the bounding box of the circle or ellipse
        the arc is part of, and the start and end angles. Other properties such as
        color and line style are inherited from cAbstractLineFigure.

        For the start and end angles, zero points east, and angles are measured in
        the counter-clockwise direction. Values outside the (0,2*PI) interval are
        accepted for both angles.

        The arc is drawn from the start angle to the end angle in <i>counter-clockwise</i>
        direction. In non-edge cases, the end angle should be in the (startAngle,
        startAngle + 2*PI) interval. If the end angle equals or is less than the
        start angle, nothing is drawn. If the end angle equals startAngle+2*PI
        or is greater than that, a full circle is drawn.
        )docstring"
    );

    py_cArcFigure.def(
        pybind11::init<const char *>(),
        pybind11::arg("name") = nullptr
    );

    py_cArcFigure.def(
        "dup",
        &omnetpp::cArcFigure::dup
    );

    py_cArcFigure.def(
        "__str__",
        &omnetpp::cArcFigure::str
    );

    py_cArcFigure.def(
        "str",
        &omnetpp::cArcFigure::str
    );

    py_cArcFigure.def(
        "parse",
        &omnetpp::cArcFigure::parse
    );

    py_cArcFigure.def(
        "moveLocal",
        &omnetpp::cArcFigure::moveLocal,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cArcFigure.def(
        "getBounds",
        &omnetpp::cArcFigure::getBounds,
        R"docstring(
        Returns the bounding box of the axis-aligned circle or ellipse
        the arc is part of.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cArcFigure.def(
        "setBounds",
        &omnetpp::cArcFigure::setBounds,
        R"docstring(
        Sets the bounding box of the axis-aligned circle or ellipse
        the arc is part of.
        )docstring",
        pybind11::arg("bounds")
    );

    py_cArcFigure.def(
        "setPosition",
        &omnetpp::cArcFigure::setPosition,
        R"docstring(
        Utility function. Moves the bounding box so that its point
        denoted by anchor will be at the given position.
        )docstring",
        pybind11::arg("position"), pybind11::arg("anchor")
    );

    py_cArcFigure.def(
        "getStartAngle",
        &omnetpp::cArcFigure::getStartAngle,
        R"docstring(
        Returns the start angle of the arc in radians. See the class
        documentation for details on how the arc is drawn.
        )docstring"
    );

    py_cArcFigure.def(
        "setStartAngle",
        &omnetpp::cArcFigure::setStartAngle,
        R"docstring(
        Sets the start angle of the arc in radians. See the class
        documentation for details on how the arc is drawn.
        )docstring",
        pybind11::arg("startAngle")
    );

    py_cArcFigure.def(
        "getEndAngle",
        &omnetpp::cArcFigure::getEndAngle,
        R"docstring(
        Returns the end angle of the arc in radians. See the class
        documentation for details on how the arc is drawn.
        )docstring"
    );

    py_cArcFigure.def(
        "setEndAngle",
        &omnetpp::cArcFigure::setEndAngle,
        R"docstring(
        Sets the end angle of the arc in radians. See the class
        documentation for details on how the arc is drawn.
        )docstring",
        pybind11::arg("endAngle")
    );
}

void bind_cPolylineFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cPolylineFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cPolylineFigure, pybind11::nodelete>
        > py_cPolylineFigure(
        m,
        "_cPolylineFigure",
        R"docstring(
        A figure that displays a line that consists of multiple connecting
        straight line segments or of a single smooth curve.

        The class stores geometry information as a sequence of points. The line
        may be *smoothed*. A smoothed line is drawn as a series of Bezier
        curves, which touch the start point of the first line segment, the
        end point of the last line segment, and the midpoints of intermediate
        line segments, while intermediate points serve as control points.
        To add corners to a smoothed curve, duplicate points.

        Additional properties such as color and line style are inherited from
        cAbstractLineFigure.
        )docstring"
    );

    py_cPolylineFigure.def(
        pybind11::init<const char *>(),
        pybind11::arg("name") = nullptr
    );

    py_cPolylineFigure.def(
        "dup",
        &omnetpp::cPolylineFigure::dup
    );

    py_cPolylineFigure.def(
        "__str__",
        &omnetpp::cPolylineFigure::str
    );

    py_cPolylineFigure.def(
        "str",
        &omnetpp::cPolylineFigure::str
    );

    py_cPolylineFigure.def(
        "parse",
        &omnetpp::cPolylineFigure::parse
    );

    py_cPolylineFigure.def(
        "moveLocal",
        &omnetpp::cPolylineFigure::moveLocal,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cPolylineFigure.def(
        "getPoints",
        &omnetpp::cPolylineFigure::getPoints,
        R"docstring(
        Returns the points that define the polyline.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cPolylineFigure.def(
        "setPoints",
        &omnetpp::cPolylineFigure::setPoints,
        R"docstring(
        Sets the points that define the polyline.
        )docstring",
        pybind11::arg("points")
    );

    py_cPolylineFigure.def(
        "getNumPoints",
        &omnetpp::cPolylineFigure::getNumPoints,
        R"docstring(
        Returns the number of points. Equivalent to len(self.getPoints())
        )docstring"
    );

    py_cPolylineFigure.def(
        "getPoint",
        &omnetpp::cPolylineFigure::getPoint,
        R"docstring(
        Returns the ith point. The index must be in the 0..getNumPoints()-1 interval.
        )docstring",
        pybind11::arg("i")
    );

    py_cPolylineFigure.def(
        "setPoint",
        &omnetpp::cPolylineFigure::setPoint,
        R"docstring(
        Sets the ith point. The index must be in the 0..getNumPoints()-1 interval.
        )docstring",
        pybind11::arg("i"), pybind11::arg("point")
    );

    py_cPolylineFigure.def(
        "addPoint",
        &omnetpp::cPolylineFigure::addPoint,
        pybind11::arg("p"),
        R"docstring(
        Appends a point.
        )docstring"
    );

    py_cPolylineFigure.def(
        "removePoint",
        &omnetpp::cPolylineFigure::removePoint,
        R"docstring(
        Removes the ith point. The index must be in the 0..getNumPoints()-1 interval.
        )docstring",
        pybind11::arg("i")
    );

    py_cPolylineFigure.def(
        "insertPoint",
        &omnetpp::cPolylineFigure::insertPoint,
        R"docstring(
        Inserts a point at position i, shifting up the point at and above index i.
        The index must be in the 0..getNumPoints()-1 interval.
        )docstring",
        pybind11::arg("i"), pybind11::arg("point")
    );

    py_cPolylineFigure.def(
        "getSmooth",
        &omnetpp::cPolylineFigure::getSmooth,
        R"docstring(
        Returns true if the figure is rendered as a smoothed curve, and
        false if as a sequence of connecting of straight line segments.
        See the class description for details on how the points define
        the curve in the smoothed case.
        )docstring"
    );

    py_cPolylineFigure.def(
        "setSmooth",
        &omnetpp::cPolylineFigure::setSmooth,
        R"docstring(
        Sets the flag that determines whether connecting straight line segments
        or a single smooth curve should be drawn. See the class description for
        details on how the points define the curve in the smoothed case.
        The default value is false (no smoothing).
        )docstring",
        pybind11::arg("smooth")
    );

    py_cPolylineFigure.def(
        "getJoinStyle",
        &omnetpp::cPolylineFigure::getJoinStyle,
        R"docstring(
        Returns the join style line segments will be connected with.
        )docstring"
    );

    py_cPolylineFigure.def(
        "setJoinStyle",
        &omnetpp::cPolylineFigure::setJoinStyle,
        R"docstring(
        Sets the join style line segments will be connected with. The
        default join style is miter.
        )docstring",
        pybind11::arg("joinStyle")
    );
}

void bind_cAbstractShapeFigure(pybind11::module &m)
{

    pybind11::class_<
        omnetpp::cAbstractShapeFigure, omnetpp::cFigure
        > py_cAbstractShapeFigure(
        m,
        "_cAbstractShapeFigure",
        R"docstring(
        AbstractShape base class for various shapes.

        This class provides line and fill color, line and fill opacity, line style,
        line width, and other properties for them. Both outline and fill are optional.

        When the outline is drawn with a width larger than one pixel, it will be
        drawn symmetrically, i.e. approximately 50-50% of its width will fall
        inside and outside the shape.

        Transformations such as scaling or skew do affect the width of the line as it
        is rendered on the canvas. Whether zooming (by the user) should also affect
        it can be controlled by setting a flag (see setZoomLineWidth()).

        The rendering of zero-width lines is currently undefined. It is attempted
        to be rendered as a one pixel wide line, regardless of transforms and zoom
        level, but it is not possible on all platform.
        )docstring"
    );

    py_cAbstractShapeFigure.def(
        "dup",
        &omnetpp::cAbstractShapeFigure::dup
    );

    py_cAbstractShapeFigure.def(
        "__str__",
        &omnetpp::cAbstractShapeFigure::str
    );

    py_cAbstractShapeFigure.def(
        "str",
        &omnetpp::cAbstractShapeFigure::str
    );

    py_cAbstractShapeFigure.def(
        "parse",
        &omnetpp::cAbstractShapeFigure::parse
    );

    py_cAbstractShapeFigure.def(
        "isFilled",
        &omnetpp::cAbstractShapeFigure::isFilled,
        R"docstring(
        Returns true if the figure rendered as filled, and false if not.
        )docstring"
    );

    py_cAbstractShapeFigure.def(
        "setFilled",
        &omnetpp::cAbstractShapeFigure::setFilled,
        R"docstring(
        Sets the flag that controls whether the figure should be filled.
        The default setting is false (unfilled).
        )docstring",
        pybind11::arg("filled")
    );

    py_cAbstractShapeFigure.def(
        "isOutlined",
        &omnetpp::cAbstractShapeFigure::isOutlined,
        R"docstring(
        Returns true if the figure's outline is to be drawn, and false if not.
        )docstring"
    );

    py_cAbstractShapeFigure.def(
        "setOutlined",
        &omnetpp::cAbstractShapeFigure::setOutlined,
        R"docstring(
        Sets the flag that controls whether the figure's outline should be
        drawn. The default setting is true.
        )docstring",
        pybind11::arg("outlined")
    );

    py_cAbstractShapeFigure.def(
        "getLineColor",
        &omnetpp::cAbstractShapeFigure::getLineColor,
        R"docstring(
        Returns the color of the figure's outline.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractShapeFigure.def(
        "setLineColor",
        &omnetpp::cAbstractShapeFigure::setLineColor,
        R"docstring(
        Sets the color of the figure's outline. The default outline color is black.
        )docstring",
        pybind11::arg("lineColor")
    );

    py_cAbstractShapeFigure.def(
        "getFillColor",
        &omnetpp::cAbstractShapeFigure::getFillColor,
        R"docstring(
        Returns the fill color of the figure.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractShapeFigure.def(
        "setFillColor",
        &omnetpp::cAbstractShapeFigure::setFillColor,
        R"docstring(
        Sets the fill color of the figure. Note that setting the fill color
        in itself does not make the figure filled, calling setFilled(true)
        is also needed. The default fill color is blue.
        )docstring",
        pybind11::arg("lineColor")
    );

    py_cAbstractShapeFigure.def(
        "getLineStyle",
        &omnetpp::cAbstractShapeFigure::getLineStyle,
        R"docstring(
        Returns the line style of the figure's outline.
        )docstring"
    );

    py_cAbstractShapeFigure.def(
        "setLineStyle",
        &omnetpp::cAbstractShapeFigure::setLineStyle,
        R"docstring(
        Sets the line style of the figure's outline (dotted, dashed, etc.)
        The default line style is solid.
        )docstring",
        pybind11::arg("lineStyle")
    );

    py_cAbstractShapeFigure.def(
        "getLineWidth",
        &omnetpp::cAbstractShapeFigure::getLineWidth,
        R"docstring(
        Returns the line width of the figure's outline. Note that the actual line
        width will also be affected by the figure transformations and,
        if zoomLineWidth is set, by the zoom level as well.
        )docstring"
    );

    py_cAbstractShapeFigure.def(
        "setLineWidth",
        &omnetpp::cAbstractShapeFigure::setLineWidth,
        R"docstring(
        Sets the line width of the figure's outline. The number must be
        positive (zero is not allowed). The default line width is 1.
        See also setZoomLineWidth().
        )docstring",
        pybind11::arg("lineWidth")
    );

    py_cAbstractShapeFigure.def(
        "getLineOpacity",
        &omnetpp::cAbstractShapeFigure::getLineOpacity,
        R"docstring(
        Returns the opacity of the figure's outline, a number in the
        [0,1] interval.
        )docstring"
    );

    py_cAbstractShapeFigure.def(
        "setLineOpacity",
        &omnetpp::cAbstractShapeFigure::setLineOpacity,
        R"docstring(
        Sets the opacity of the figure's outline. The argument must be a
        number in the [0,1] interval. The default opacity is 1.0.
        )docstring",
        pybind11::arg("lineOpacity")
    );

    py_cAbstractShapeFigure.def(
        "getFillOpacity",
        &omnetpp::cAbstractShapeFigure::getFillOpacity,
        R"docstring(
        Returns the opacity of the figure's fill, a number in the
        [0,1] interval.
        )docstring"
    );

    py_cAbstractShapeFigure.def(
        "setFillOpacity",
        &omnetpp::cAbstractShapeFigure::setFillOpacity,
        R"docstring(
        Sets the opacity of the figure's fill. The argument must be a
        number in the [0,1] interval. The default opacity is 1.0.
        )docstring",
        pybind11::arg("lineOpacity")
    );

    py_cAbstractShapeFigure.def(
        "getZoomLineWidth",
        &omnetpp::cAbstractShapeFigure::getZoomLineWidth,
        R"docstring(
        Returns true if zoom level affects the line width the figure's
        outline is drawn with, and false if it does not.
        )docstring"
    );

    py_cAbstractShapeFigure.def(
        "setZoomLineWidth",
        &omnetpp::cAbstractShapeFigure::setZoomLineWidth,
        R"docstring(
        Sets the flag that controls whether zoom level should affect
        the line width the figure's outline is drawn with. The default
        setting is false, i.e. zoom does not affect line width.
        )docstring",
        pybind11::arg("lineWidth")
    );
}

void bind_cRectangleFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cRectangleFigure, omnetpp::cAbstractShapeFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cRectangleFigure, pybind11::nodelete>
        > py_cRectangleFigure(
        m,
        "_cRectangleFigure",
        R"docstring(
        A figure that displays a rectangle, with optionally rounded corners.

        As with all shape figures, drawing of both the outline and the fill
        are optional. Line and fill color, and several other properties are
        inherited from cAbstractShapeFigure.
        )docstring"
    );

    py_cRectangleFigure.def(
        pybind11::init<const char*>(),
        pybind11::arg("name") = nullptr
    );

    py_cRectangleFigure.def(
        "dup",
        &omnetpp::cRectangleFigure::dup
    );

    py_cRectangleFigure.def(
        "__str__",
        &omnetpp::cRectangleFigure::str
    );

    py_cRectangleFigure.def(
        "str",
        &omnetpp::cRectangleFigure::str
    );

    py_cRectangleFigure.def(
        "parse",
        &omnetpp::cRectangleFigure::parse
    );

    py_cRectangleFigure.def(
        "moveLocal",
        &omnetpp::cRectangleFigure::moveLocal,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cRectangleFigure.def(
        "getBounds",
        &omnetpp::cRectangleFigure::getBounds,
        R"docstring(
        Returns the rectangle that defines the figure's geometry.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cRectangleFigure.def(
        "setBounds",
        &omnetpp::cRectangleFigure::setBounds,
        R"docstring(
        Sets the rectangle that defines the figure's geometry.
        )docstring",
        pybind11::arg("bounds")
    );

    py_cRectangleFigure.def(
        "setPosition",
        &omnetpp::cRectangleFigure::setPosition,
        R"docstring(
        Utility function. Moves the bounding box so that its point
        denoted by anchor will be at the given position.
        )docstring",
        pybind11::arg("position"), pybind11::arg("anchor")
    );

    py_cRectangleFigure.def(
        "setCornerRadius",
        &omnetpp::cRectangleFigure::setCornerRadius,
        R"docstring(
        Sets both the horizontal and vertical (x and y) corner radii to
        the same value.
        )docstring",
        pybind11::arg("r")
    );

    py_cRectangleFigure.def(
        "getCornerRx",
        &omnetpp::cRectangleFigure::getCornerRx,
        R"docstring(
        Returns the horizontal radius of the rectangle's rounded corners.
        )docstring"
    );

    py_cRectangleFigure.def(
        "setCornerRx",
        &omnetpp::cRectangleFigure::setCornerRx,
        R"docstring(
        Sets the horizontal radius of the rectangle's rounded corners.
        Specify 0 to turn off rounded corners. The default is 0.
        )docstring",
        pybind11::arg("rx")
    );

    py_cRectangleFigure.def(
        "getCornerRy",
        &omnetpp::cRectangleFigure::getCornerRy,
        R"docstring(
        Returns the vertical radius of the rectangle's rounded corners.
        )docstring"
    );

    py_cRectangleFigure.def(
        "setCornerRy",
        &omnetpp::cRectangleFigure::setCornerRy,
        R"docstring(
        Sets the vertical radius of the rectangle's rounded corners.
        Specify 0 to turn off rounded corners. The default is 0.
        )docstring",
        pybind11::arg("ry")
    );
}

void bind_cOvalFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cOvalFigure, omnetpp::cAbstractShapeFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cOvalFigure, pybind11::nodelete>
        > py_cOvalFigure(
        m,
        "_cOvalFigure",
        R"docstring(
        As with all shape figures, drawing of both the outline and the fill are
        optional. Line and fill color, and several other properties are inherited
        from cAbstractShapeFigure.
        )docstring"
    );

    py_cOvalFigure.def(
        pybind11::init<const char *>(),
        pybind11::arg("name")
    );

    py_cOvalFigure.def(
        "dup",
        &omnetpp::cOvalFigure::dup
    );

    py_cOvalFigure.def(
        "__str__",
        &omnetpp::cOvalFigure::str
    );

    py_cOvalFigure.def(
        "str",
        &omnetpp::cOvalFigure::str
    );

    py_cOvalFigure.def(
        "parse",
        &omnetpp::cOvalFigure::parse
    );

    py_cOvalFigure.def(
        "moveLocal",
        &omnetpp::cOvalFigure::moveLocal,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cOvalFigure.def(
        "getBounds",
        &omnetpp::cOvalFigure::getBounds,
        R"docstring(
        Returns the rectangle that defines the figure's geometry.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cOvalFigure.def(
        "setBounds",
        &omnetpp::cOvalFigure::setBounds,
        R"docstring(
        Sets the rectangle that defines the figure's geometry.
        )docstring",
        pybind11::arg("bounds")
    );

    py_cOvalFigure.def(
        "setPosition",
        &omnetpp::cOvalFigure::setPosition,
        R"docstring(
        Utility function. Moves the bounding box so that its point
        denoted by anchor will be at the given position.
        )docstring",
        pybind11::arg("position"), pybind11::arg("anchor")
    );
}

void bind_cRingFigure(pybind11::module &m)
{
    // cRingFigure
    pybind11::class_<
        omnetpp::cRingFigure, omnetpp::cAbstractShapeFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cRingFigure, pybind11::nodelete>
        > py_cRingFigure(
        m,
        "_cRingFigure",
        R"docstring(
        The inner/outer circles (or ellipses) form the outline, and the area between
        them is filled. As with all shape figures, drawing of both the outline and
        the fill are optional. Line and fill color, and several other properties are
        inherited from cAbstractShapeFigure.
        )docstring"
    );

    py_cRingFigure.def(
        pybind11::init<const char*>(),
        pybind11::arg("name") = nullptr
    );

    py_cRingFigure.def(
        "dup",
        &omnetpp::cRingFigure::dup
    );

    py_cRingFigure.def(
        "__str__",
        &omnetpp::cRingFigure::str
    );

    py_cRingFigure.def(
        "str",
        &omnetpp::cRingFigure::str
    );

    py_cRingFigure.def(
        "parse",
        &omnetpp::cRingFigure::parse
    );

    py_cRingFigure.def(
        "moveLocal",
        &omnetpp::cRingFigure::moveLocal,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cRingFigure.def(
        "getBounds",
        &omnetpp::cRingFigure::getBounds,
        R"docstring(
        Returns the rectangle that defines the ring's outer circle or ellipse.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cRingFigure.def(
        "setBounds",
        &omnetpp::cRingFigure::setBounds,
        R"docstring(
        Sets the rectangle that defines the ring's outer circle or ellipse.
        )docstring",
        pybind11::arg("bounds")
    );

    py_cRingFigure.def(
        "setPosition",
        &omnetpp::cRingFigure::setPosition,
        R"docstring(
        Utility function. Moves the bounding box so that its point
        denoted by anchor will be at the given position.
        )docstring",
        pybind11::arg("position"), pybind11::arg("anchor")
    );

    py_cRingFigure.def(
        "setInnerRadius",
        &omnetpp::cRingFigure::setInnerRadius,
        R"docstring(
        Sets the ring's inner curve to a circle with the given radius.
        This method sets the horizontal and vertical inner radii to the
        same value.
        )docstring",
        pybind11::arg("r")
    );

    py_cRingFigure.def(
        "getInnerRx",
        &omnetpp::cRingFigure::getInnerRx,
        R"docstring(
        Returns the horizontal radius of the ring's inner circle or ellipse.
        )docstring"
    );

    py_cRingFigure.def(
        "setInnerRx",
        &omnetpp::cRingFigure::setInnerRx,
        R"docstring(
        Sets the horizontal radius of the ring's inner circle or ellipse.
        )docstring",
        pybind11::arg("rx")
    );

    py_cRingFigure.def(
        "getInnerRy",
        &omnetpp::cRingFigure::getInnerRy,
        R"docstring(
        Returns the vertical radius of the ring's inner circle or ellipse.
        )docstring"
    );

    py_cRingFigure.def(
        "setInnerRy",
        &omnetpp::cRingFigure::setInnerRy,
        R"docstring(
        Sets the vertical radius of the ring's inner circle or ellipse.
        )docstring",
        pybind11::arg("ry")
    );
}

void bind_cPieSliceFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cPieSliceFigure, omnetpp::cAbstractShapeFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cPieSliceFigure, pybind11::nodelete>
        > py_cPieSliceFigure(
        m,
        "_cPieSliceFigure",
        R"docstring(
        A figure that displays a pie slice, that is, a section of an
        axis-aligned disc or filled ellipse.

        A pie slice is determined by the bounding box of the full disc or ellipse,
        and a start and an end angle. The outline of the pie slice in non-degenerate
        cases consists of an arc, and two straight lines from the center of the pie
        to the end points of the arc.

        For the start and end angles, zero points east, and angles are measured in
        the counter-clockwise direction. Values outside the (0,2*PI) interval are
        accepted for both angles.

        As with all shape figures, drawing of both the outline and the fill are
        optional. Line and fill color, and several other properties are inherited
        from cAbstractShapeFigure.

        The pie slice is drawn from the start angle to the end angle in <i>counter-
        clockwise</i> direction. In non-degenerate cases, the end angle should be
        in the (startAngle, startAngle + 2*PI) interval. If the end angle equals to
        or is less than the start angle, only a straight line is drawn from the
        center to the circumference. If the end angle equals startAngle+2*PI
        or is greater than that, a full circle or ellipse, and a straight line from
        the center to the circumference at angle endAngle is drawn.
        )docstring"
    );

    py_cPieSliceFigure.def(
        pybind11::init<const char*>(),
        pybind11::arg("name") = nullptr
    );

    py_cPieSliceFigure.def(
        "dup",
        &omnetpp::cPieSliceFigure::dup
    );

    py_cPieSliceFigure.def(
        "__str__",
        &omnetpp::cPieSliceFigure::str
    );

    py_cPieSliceFigure.def(
        "str",
        &omnetpp::cPieSliceFigure::str
    );

    py_cPieSliceFigure.def(
        "parse",
        &omnetpp::cPieSliceFigure::parse
    );

    py_cPieSliceFigure.def(
        "moveLocal",
        &omnetpp::cPieSliceFigure::moveLocal,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cPieSliceFigure.def(
        "getBounds",
        &omnetpp::cPieSliceFigure::getBounds,
        R"docstring(
        Returns the bounding box of the axis-aligned circle or ellipse
        the pie slice is part of.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cPieSliceFigure.def(
        "setBounds",
        &omnetpp::cPieSliceFigure::setBounds,
        R"docstring(
        Sets the bounding box of the axis-aligned circle or ellipse
        the pie slice is part of.
        )docstring",
        pybind11::arg("bounds")
    );

    py_cPieSliceFigure.def(
        "setPosition",
        &omnetpp::cPieSliceFigure::setPosition,
        R"docstring(
        Utility function. Moves the bounding box so that its point
        denoted by anchor will be at the given position.
        )docstring",
        pybind11::arg("position"), pybind11::arg("anchor")
    );

    py_cPieSliceFigure.def(
        "getStartAngle",
        &omnetpp::cPieSliceFigure::getStartAngle,
        R"docstring(
        Returns the start angle of the pie slice in radians. See the class
        documentation for details on how the pie slice is drawn.
        )docstring"
    );

    py_cPieSliceFigure.def(
        "setStartAngle",
        &omnetpp::cPieSliceFigure::setStartAngle,
        R"docstring(
        Sets the start angle of the pie slice in radians. See the class
        documentation for details on how the pie slice is drawn.
        )docstring",
        pybind11::arg("startAngle")
    );

    py_cPieSliceFigure.def(
        "getEndAngle",
        &omnetpp::cPieSliceFigure::getEndAngle,
        R"docstring(
        Returns the end angle of the pie slice in radians. See the class
        documentation for details on how the pie slice is drawn.
        )docstring"
    );

    py_cPieSliceFigure.def(
        "setEndAngle",
        &omnetpp::cPieSliceFigure::setEndAngle,
        R"docstring(
        Sets the end angle of the pie slice in radians. See the class
        documentation for details on how the pie slice is drawn.
        )docstring",
        pybind11::arg("startAngle")
    );
}

void bind_cPolygonFigure(pybind11::module &m)
{

    pybind11::class_<
        omnetpp::cPolygonFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cPolygonFigure, pybind11::nodelete>
        > py_cPolygonFigure(
        m,
        "_cPolygonFigure",
        R"docstring(
        A figure that displays a (closed) polygon, determined by a sequence of points.

        The polygon may be <i>smoothed</i>. A smoothed polygon is drawn as a series
        of cubic Bezier curves, where the curves touch the midpoints of the sides,
        and vertices serve as control points. As with all shape figures, drawing of
        both the outline and the fill are optional. The drawing of filled self-
        intersecting polygons is controlled by the <i>fill rule</i> property.
        Line and fill color, and several other properties are inherited from
        cAbstractShapeFigure.
        )docstring"
    );

    py_cPolygonFigure.def(
        pybind11::init<const char *>(),
        pybind11::arg("name") = nullptr
    );

    py_cPolygonFigure.def(
        "dup",
        &omnetpp::cPolygonFigure::dup
    );

    py_cPolygonFigure.def(
        "__str__",
        &omnetpp::cPolygonFigure::str
    );

    py_cPolygonFigure.def(
        "str",
        &omnetpp::cPolygonFigure::str
    );

    py_cPolygonFigure.def(
        "parse",
        &omnetpp::cPolygonFigure::parse
    );

    py_cPolygonFigure.def(
        "moveLocal",
        &omnetpp::cPolygonFigure::moveLocal,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cPolygonFigure.def(
        "getPoints",
        &omnetpp::cPolygonFigure::getPoints,
        R"docstring(
        Returns the points that define the polyline.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cPolygonFigure.def(
        "setPoints",
        &omnetpp::cPolygonFigure::setPoints,
        R"docstring(
        Sets the points that define the polyline.
        )docstring",
        pybind11::arg("points")
    );

    py_cPolygonFigure.def(
        "getNumPoints",
        &omnetpp::cPolygonFigure::getNumPoints,
        R"docstring(
        Returns the number of points. Equivalent to len(self.getPoints())
        )docstring"
    );

    py_cPolygonFigure.def(
        "getPoint",
        &omnetpp::cPolygonFigure::getPoint,
        R"docstring(
        Returns the ith point. The index must be in the 0..getNumPoints()-1
        interval.
        )docstring",
        pybind11::arg("i")
    );

    py_cPolygonFigure.def(
        "setPoint",
        &omnetpp::cPolygonFigure::setPoint,
        R"docstring(
        Sets the ith point. The index must be in the 0..getNumPoints()-1 interval.
        )docstring",
        pybind11::arg("i"), pybind11::arg("point")
    );

    py_cPolygonFigure.def(
        "addPoint",
        &omnetpp::cPolygonFigure::addPoint,
        R"docstring(
        Appends a point.
        )docstring",
        pybind11::arg("p")
    );

    py_cPolygonFigure.def(
        "removePoint",
        &omnetpp::cPolygonFigure::removePoint,
        R"docstring(
        Removes the ith point. The index must be in the 0..getNumPoints()-1 interval.
        )docstring",
        pybind11::arg("i")
    );

    py_cPolygonFigure.def(
        "insertPoint",
        &omnetpp::cPolygonFigure::insertPoint,
        R"docstring(
        Inserts a point at position i, shifting up the point at and above index i.
        The index must be in the 0..getNumPoints()-1 interval.
        )docstring",
        pybind11::arg("i"), pybind11::arg("point")
    );

    py_cPolygonFigure.def(
        "getSmooth",
        &omnetpp::cPolygonFigure::getSmooth,
        R"docstring(
        Returns true if the figure is rendered as a closed smooth curve, and
        false if as a polygon. See the class description for details.
        )docstring"
    );

    py_cPolygonFigure.def(
        "setSmooth",
        &omnetpp::cPolygonFigure::setSmooth,
        R"docstring(
        Sets the flag that determines whether a polygon or a smooth closed
        curve should be drawn. See the class description for details on how
        the points define the curve in the smoothed case.
        )docstring",
        pybind11::arg("smooth")
    );

    py_cPolygonFigure.def(
        "getJoinStyle",
        &omnetpp::cPolygonFigure::getJoinStyle,
        R"docstring(
        Returns the join style line segments will be connected with.
        )docstring"
    );

    py_cPolygonFigure.def(
        "setJoinStyle",
        &omnetpp::cPolygonFigure::setJoinStyle,
        R"docstring(
        Sets the join style line segments will be connected with. The
        default join style is miter.
        )docstring",
        pybind11::arg("joinStyle")
    );

    py_cPolygonFigure.def(
        "getFillRule",
        &omnetpp::cPolygonFigure::getFillRule,
        R"docstring(
        Returns the fill rule of the polygon. This attribute is only
        important with filled, self-intersecting polygons. The
        interpretation of the fill rule is consistent with its SVG
        definition.
        )docstring"
    );

    py_cPolygonFigure.def(
        "setFillRule",
        &omnetpp::cPolygonFigure::setFillRule,
        R"docstring(
        Sets the fill rule of the polygon. This attribute is only important
        with filled, self-intersecting polygons.  The interpretation of
        the fill rule is consistent with its SVG definition.

        The default fill rule is evenodd.
        )docstring",
        pybind11::arg("fillRule")
    );
}

void bind_cPathFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cPathFigure, omnetpp::cAbstractShapeFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cPathFigure, pybind11::nodelete>
    > py_cPathFigure(
        m,
        "_cPathFigure",
        R"docstring(
        A figure that displays a "path", a complex shape or line modeled
        after SVG paths.

        A path is may consist of any number of straight line segments, Bezier curves
        and arcs. The path can be disjoint as well. Closed paths may be filled.
        The drawing of filled self-intersecting polygons is controlled by the
        *fill rule* property. Line and fill color, and several other properties
        are inherited from cAbstractShapeFigure.

        The path may be specified with a string similar to an SVG path, or assembled
        by calling methods that append new segments (straight lines, arcs or Bezier
        curves) to the existing path.
        )docstring"
    );

    pybind11::class_<omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "PathItem",
        "Represents an item in a cPathFigure path.")
        .def(pybind11::init<>())
        .def_readwrite("code", &omnetpp::cPathFigure::PathItem::code);

    pybind11::class_<omnetpp::cPathFigure::MoveTo, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "MoveTo",
        "Represents the 'M' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("x", &omnetpp::cPathFigure::MoveTo::x)
        .def_readwrite("y", &omnetpp::cPathFigure::MoveTo::y);

    pybind11::class_<omnetpp::cPathFigure::MoveRel, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "MoveRel",
        "Represents the 'm' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("dx", &omnetpp::cPathFigure::MoveRel::dx)
        .def_readwrite("dy", &omnetpp::cPathFigure::MoveRel::dy);

    pybind11::class_<omnetpp::cPathFigure::LineTo, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "LineTo",
        "Represents the 'L' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("x", &omnetpp::cPathFigure::LineTo::x)
        .def_readwrite("y", &omnetpp::cPathFigure::LineTo::y);

    pybind11::class_<omnetpp::cPathFigure::LineRel, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "LineRel",
        "Represents the 'l' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("dx", &omnetpp::cPathFigure::LineRel::dx)
        .def_readwrite("dy", &omnetpp::cPathFigure::LineRel::dy);

    pybind11::class_<omnetpp::cPathFigure::HorizontalLineTo, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "HorizontalLineTo",
        "Represents the 'H' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("x", &omnetpp::cPathFigure::HorizontalLineTo::x);

    pybind11::class_<omnetpp::cPathFigure::HorizontalLineRel, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "HorizontalLineRel",
        "Represents the 'h' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("dx", &omnetpp::cPathFigure::HorizontalLineRel::dx);

    pybind11::class_<omnetpp::cPathFigure::VerticalLineTo, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "VerticalLineTo",
        "Represents the 'V' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("y", &omnetpp::cPathFigure::VerticalLineTo::y);

    pybind11::class_<omnetpp::cPathFigure::VerticalLineRel, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "VerticalLineRel",
        "Represents the 'v' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("dy", &omnetpp::cPathFigure::VerticalLineRel::dy);

    pybind11::class_<omnetpp::cPathFigure::ArcTo, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "ArcTo",
        "Represents the 'A' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("rx", &omnetpp::cPathFigure::ArcTo::rx)
        .def_readwrite("ry", &omnetpp::cPathFigure::ArcTo::ry)
        .def_readwrite("phi", &omnetpp::cPathFigure::ArcTo::phi)
        .def_readwrite("largeArc", &omnetpp::cPathFigure::ArcTo::largeArc)
        .def_readwrite("sweep", &omnetpp::cPathFigure::ArcTo::sweep)
        .def_readwrite("x", &omnetpp::cPathFigure::ArcTo::x)
        .def_readwrite("y", &omnetpp::cPathFigure::ArcTo::y);

    pybind11::class_<omnetpp::cPathFigure::ArcRel, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "ArcRel",
        "Represents the 'a' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("rx", &omnetpp::cPathFigure::ArcRel::rx)
        .def_readwrite("ry", &omnetpp::cPathFigure::ArcRel::ry)
        .def_readwrite("phi", &omnetpp::cPathFigure::ArcRel::phi)
        .def_readwrite("largeArc", &omnetpp::cPathFigure::ArcRel::largeArc)
        .def_readwrite("sweep", &omnetpp::cPathFigure::ArcRel::sweep)
        .def_readwrite("dx", &omnetpp::cPathFigure::ArcRel::dx)
        .def_readwrite("dy", &omnetpp::cPathFigure::ArcRel::dy);

    pybind11::class_<omnetpp::cPathFigure::CurveTo, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "CurveTo",
        "Represents the 'Q' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("x1", &omnetpp::cPathFigure::CurveTo::x1)
        .def_readwrite("y1", &omnetpp::cPathFigure::CurveTo::x1)
        .def_readwrite("x", &omnetpp::cPathFigure::CurveTo::x)
        .def_readwrite("y", &omnetpp::cPathFigure::CurveTo::y);

    pybind11::class_<omnetpp::cPathFigure::CurveRel, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "CurveRel",
        "Represents the 'q' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("dx1", &omnetpp::cPathFigure::CurveRel::dx1)
        .def_readwrite("dy1", &omnetpp::cPathFigure::CurveRel::dx1)
        .def_readwrite("dx", &omnetpp::cPathFigure::CurveRel::dx)
        .def_readwrite("dy", &omnetpp::cPathFigure::CurveRel::dy);

    pybind11::class_<omnetpp::cPathFigure::SmoothCurveTo, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "SmoothCurveTo",
        "Represents the 'T' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("x", &omnetpp::cPathFigure::SmoothCurveTo::x)
        .def_readwrite("y", &omnetpp::cPathFigure::SmoothCurveTo::y);

    pybind11::class_<omnetpp::cPathFigure::SmoothCurveRel, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "SmoothCurveRel",
        "Represents the 't' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("dx", &omnetpp::cPathFigure::SmoothCurveRel::dx)
        .def_readwrite("dy", &omnetpp::cPathFigure::SmoothCurveRel::dy);

    pybind11::class_<omnetpp::cPathFigure::CubicBezierCurveTo, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "CubicBezierCurveTo",
        "Represents the 'C' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("x1", &omnetpp::cPathFigure::CubicBezierCurveTo::x1)
        .def_readwrite("y1", &omnetpp::cPathFigure::CubicBezierCurveTo::y1)
        .def_readwrite("x2", &omnetpp::cPathFigure::CubicBezierCurveTo::x2)
        .def_readwrite("y2", &omnetpp::cPathFigure::CubicBezierCurveTo::y2)
        .def_readwrite("x", &omnetpp::cPathFigure::CubicBezierCurveTo::x)
        .def_readwrite("y", &omnetpp::cPathFigure::CubicBezierCurveTo::y);

    pybind11::class_<omnetpp::cPathFigure::CubicBezierCurveRel, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "CubicBezierCurveRel",
        "Represents the 'c' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("dx1", &omnetpp::cPathFigure::CubicBezierCurveRel::dx1)
        .def_readwrite("dy1", &omnetpp::cPathFigure::CubicBezierCurveRel::dy1)
        .def_readwrite("dx2", &omnetpp::cPathFigure::CubicBezierCurveRel::dx2)
        .def_readwrite("dy2", &omnetpp::cPathFigure::CubicBezierCurveRel::dy2)
        .def_readwrite("dx", &omnetpp::cPathFigure::CubicBezierCurveRel::dx)
        .def_readwrite("dy", &omnetpp::cPathFigure::CubicBezierCurveRel::dy);

    pybind11::class_<omnetpp::cPathFigure::SmoothCubicBezierCurveTo, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "SmoothCubicBezierCurveTo",
        "Represents the 'S' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("x2", &omnetpp::cPathFigure::SmoothCubicBezierCurveTo::x2)
        .def_readwrite("y2", &omnetpp::cPathFigure::SmoothCubicBezierCurveTo::y2)
        .def_readwrite("x", &omnetpp::cPathFigure::SmoothCubicBezierCurveTo::x)
        .def_readwrite("y", &omnetpp::cPathFigure::SmoothCubicBezierCurveTo::y);

    pybind11::class_<omnetpp::cPathFigure::SmoothCubicBezierCurveRel, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "SmoothCubicBezierCurveRel",
        "Represents the 's' path command with parameters")
        .def(pybind11::init<>())
        .def_readwrite("dx2", &omnetpp::cPathFigure::SmoothCubicBezierCurveRel::dx2)
        .def_readwrite("dy2", &omnetpp::cPathFigure::SmoothCubicBezierCurveRel::dy2)
        .def_readwrite("dx", &omnetpp::cPathFigure::SmoothCubicBezierCurveRel::dx)
        .def_readwrite("dy", &omnetpp::cPathFigure::SmoothCubicBezierCurveRel::dy);

    pybind11::class_<omnetpp::cPathFigure::ClosePath, omnetpp::cPathFigure::PathItem>(
        py_cPathFigure,
        "ClosePath",
        "Represents the 'Z' path command")
        .def(pybind11::init<>());

    py_cPathFigure.def(
        pybind11::init<const char *>(),
        pybind11::arg("name") = nullptr
    );

    py_cPathFigure.def(
        "dup",
        &omnetpp::cPathFigure::dup
    );

    py_cPathFigure.def(
        "__str__",
        &omnetpp::cPathFigure::str
    );

    py_cPathFigure.def(
        "str",
        &omnetpp::cPathFigure::str
    );

    py_cPathFigure.def(
        "parse",
        &omnetpp::cPathFigure::parse
    );

    py_cPathFigure.def(
        "moveLocal",
        &omnetpp::cPathFigure::moveLocal,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cPathFigure.def(
        "getJoinStyle",
        &omnetpp::cPathFigure::getJoinStyle,
        R"docstring(
        Returns the join style line segments will be connected with.
        )docstring"
    );

    py_cPathFigure.def(
        "setJoinStyle",
        &omnetpp::cPathFigure::setJoinStyle,
        R"docstring(
        Sets the join style line segments will be connected with.
        The default join style is miter.
        )docstring",
        pybind11::arg("joinStyle")
    );

    py_cPathFigure.def(
        "getCapStyle",
        &omnetpp::cPathFigure::getCapStyle,
        R"docstring(
        Returns the cap style for the figure's line.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cPathFigure.def(
        "setCapStyle",
        &omnetpp::cPathFigure::setCapStyle,
        R"docstring(
        Sets the cap style for the figure's line. The default cap style is butt.
        )docstring",
        pybind11::arg("capStyle")
    );

    py_cPathFigure.def(
        "getFillRule",
        &omnetpp::cPathFigure::getFillRule,
        R"docstring(
        Returns the fill rule of the path. This attribute is only important
        with closed, filled, self-intersecting paths (and those that contain
        such parts). The interpretation of the fill rule is consistent
        with its SVG definition.
        )docstring"
    );

    py_cPathFigure.def(
        "setFillRule",
        &omnetpp::cPathFigure::setFillRule,
        R"docstring(
        Sets the fill rule of the path. This attribute is only important
        with closed, filled, self-intersecting paths (and those that contain
        such parts). The interpretation of the fill rule is consistent
        with its SVG definition.

        The default fill rule is evenodd.
        )docstring",
        pybind11::arg("fillRule")
    );

    py_cPathFigure.def(
        "getOffset",
        &omnetpp::cPathFigure::getOffset,
        R"docstring(
        Returns the coordinate offset of the path. A nonzero offset
        causes the path to appear translated by the given amount in the
        local coordinate system.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cPathFigure.def(
        "setOffset",
        &omnetpp::cPathFigure::setOffset,
        R"docstring(
        Sets the coordinate offset of the path. Setting a nonzero offset
        causes the path to appear translated by the given amount in the
        local coordinate system. The elements of the path remain unchanged.
        This method primarily exists so that moveLocal() can be a
        constant-time operation.
        )docstring",
        pybind11::arg("offset")
    );

    py_cPathFigure.def(
        "getPath",
        &omnetpp::cPathFigure::getPath,
        R"docstring(
        Returns the path as a string.
        )docstring"
    );

    py_cPathFigure.def(
        "setPath",
        &omnetpp::cPathFigure::setPath,
        R"docstring(
        Sets the path from a string. It will be parsed, made available
        via the getNumPathItems(), getPathItem() methods, and may be
        further modified with the "add" methods.
        )docstring",
        pybind11::arg("path")
    );

    py_cPathFigure.def(
        "getNumPathItems",
        &omnetpp::cPathFigure::getNumPathItems,
        R"docstring(
        Returns the number of path items.
        )docstring"
    );

    py_cPathFigure.def(
        "getPathItem",
        &omnetpp::cPathFigure::getPathItem,
        R"docstring(
        Returns the kth path item. The returned item may be cast to the
        appropriate subtype (MoveTo, MoveRel, etc.) after examining
        its `code` field.
        )docstring",
        pybind11::arg("k")
    );

    py_cPathFigure.def(
        "clearPath",
        &omnetpp::cPathFigure::clearPath,
        R"docstring(
        Clears the path by discarding its path items.
        )docstring"
    );

    py_cPathFigure.def(
        "addMoveTo",
        &omnetpp::cPathFigure::addMoveTo,
        R"docstring(
        Appends a MoveTo item to the path. It will move the "pen" to (x,y),
        and begin a new subpath. Paths should begin with a MoveTo.

        SVG equivalent: "M <x> <y>"
        )docstring",
        pybind11::arg("x"), pybind11::arg("y")
    );

    py_cPathFigure.def(
        "addMoveRel",
        &omnetpp::cPathFigure::addMoveRel,
        R"docstring(
        Appends a MoveRel item to the path. It will move the "pen" from
        the current point (lastx,lasty) to (lastx+dx,lasty+dy), and begin
        a new subpath.

        SVG equivalent: "m <dx> <dy>"
        )docstring",
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cPathFigure.def(
        "addLineTo",
        &omnetpp::cPathFigure::addLineTo,
        R"docstring(
        Appends a LineTo item to the path. It will draw a line from the last
        point to (x,y).

        SVG equivalent: "L <x> <y>"
        )docstring",
        pybind11::arg("x"), pybind11::arg("y")
    );

    py_cPathFigure.def(
        "addLineRel",
        &omnetpp::cPathFigure::addLineRel,
        R"docstring(
        Appends a LineRel item to the path. It will draw a line
        from the current point (lastx, lasty) to (lastx+dx, lasty+dy).

        SVG equivalent: "l <dx> <dy>"
        )docstring",
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cPathFigure.def(
        "addHorizontalLineTo",
        &omnetpp::cPathFigure::addHorizontalLineTo,
        R"docstring(
        Appends a HorizontalLineTo item to the path. It will draw a line
        from the current point (lastx, lasty) to (x, lasty).

        SVG equivalent: "H <x>"
        )docstring",
        pybind11::arg("x")
    );

    py_cPathFigure.def(
        "addHorizontalLineRel",
        &omnetpp::cPathFigure::addHorizontalLineRel,
        R"docstring(
        Appends a HorizontalLineRel item to the path. It will draw a line
        from the current point (lastx, lasty) to (lastx+dx, lasty).

        SVG equivalent: "h <dx>"
        )docstring",
        pybind11::arg("dx")
    );

    py_cPathFigure.def(
        "addVerticalLineTo",
        &omnetpp::cPathFigure::addVerticalLineTo,
        R"docstring(
        Appends a VerticalLineTo item to the path. It will draw a line
        from the current point (lastx, lasty) to the (lastx, y) point.

        SVG equivalent: "V <y>"
        )docstring",
        pybind11::arg("y")
    );

    py_cPathFigure.def(
        "addVerticalLineRel",
        &omnetpp::cPathFigure::addVerticalLineRel,
        R"docstring(
        Appends a VerticalLineRel item to the path. It will draw a line
        from the current point (lastx, lasty) to (lastx, lasty+dy).

        SVG equivalent: "v <dy>"
        )docstring",
        pybind11::arg("dy")
    );

    py_cPathFigure.def(
        "addArcTo",
        &omnetpp::cPathFigure::addArcTo,
        R"docstring(
        Appends an ArcTo item to the path. It will draw an arc from the last
        point to the (x,y) point. The size and orientation of the ellipse
        are defined by two radii (rx, ry) and a phi rotation angle. The
        center of the ellipse is calculated automatically to satisfy the
        constraints imposed by the other parameters. The largeArc and sweep
        flags determine which of the four possible arcs are chosen. If
        largeArc=true, then one of the larger (greater than 180 degrees)
        arcs are chosen, otherwise one of the the smaller ones. If
        sweep=true, the arc will be drawn in the "positive-angle" direction,
        otherwise in the negative-angle direction.

        SVG equivalent: "A <rx> <ry> <phi> <largeArc> <sweep> <x> <y>"
        )docstring",
        pybind11::arg("rx"), pybind11::arg("ry"), pybind11::arg("phi"),
        pybind11::arg("largeArc"), pybind11::arg("sweep"), pybind11::arg("x"), pybind11::arg("y")
    );

    py_cPathFigure.def(
        "addArcRel",
        &omnetpp::cPathFigure::addArcRel,
        R"docstring(
        Appends an ArcRel item to the path. It will draw an arc from the last
        point (lastx, lasty) to (lastx+dx, lasty+dy). Just as with
        addArcTo(), the size and orientation of the ellipse are defined
        by two radii (rx, ry) and a phi rotation angle. The center of the
        ellipse is calculated automatically to satisfy the constraints
        imposed by the other parameters. The largeArc and sweep flags
        determine which of the four possible arcs are chosen. If
        largeArc=true, then one of the larger (greater than 180 degrees)
        arcs are chosen, otherwise one of the the smaller ones. If
        sweep=true, the arc will be drawn in the "positive-angle" direction,
        otherwise in the negative-angle direction.

        SVG equivalent: "a <rx> <ry> <phi> <largeArc> <sweep> <dx> <dy>"
        )docstring",
        pybind11::arg("rx"), pybind11::arg("ry"), pybind11::arg("phi"),
        pybind11::arg("largeArc"), pybind11::arg("sweep"), pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cPathFigure.def(
        "addCurveTo",
        &omnetpp::cPathFigure::addCurveTo,
        R"docstring(
        Appends a CurveTo item to the path. It will draw a quadratic Bezier curve
        from the current point to (x,y) using (x1,y1) as the control point.

        SVG equivalent: "Q <x1> <y1> <x> <y>"
        )docstring",
        pybind11::arg("x1"), pybind11::arg("y1"), pybind11::arg("x"), pybind11::arg("y")
    );

    py_cPathFigure.def(
        "addCurveRel",
        &omnetpp::cPathFigure::addCurveRel,
        R"docstring(
        Appends a CurveRel item to the path. It will draw a quadratic Bezier curve
        from the current point (lastx, lasty) to (lastx+dx, lasty+dy) using
        (lastx+dx1, lasty+dy1) as the control point.

        SVG equivalent: "q <dx1> <dy1> <dx> <dy>"
        )docstring",
        pybind11::arg("dx1"), pybind11::arg("dy1"), pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cPathFigure.def(
        "addSmoothCurveTo",
        &omnetpp::cPathFigure::addSmoothCurveTo,
        R"docstring(
        Appends a SmoothCurveTo item to the path. It will draw a quadratic
        Bezier curve from the current point to (x,y). The control point is
        assumed to be the reflection of the control point on the previous
        command relative to the current point. (If there is no previous
        command or if the previous command was not a quadratic Bezier curve
        command such as Q, q, T or t, assume the control point is coincident
        with the current point.)

        SVG equivalent: "T <x> <y>"
        )docstring",
        pybind11::arg("x"), pybind11::arg("y")
    );

    py_cPathFigure.def(
        "addSmoothCurveRel",
        &omnetpp::cPathFigure::addSmoothCurveRel,
        R"docstring(
        Appends a SmoothCurveRel item to the path. It will draw a quadratic
        Bezier curve from the current point (lastx, lasty) to (last+dx, lasty+dy).
        The control point is assumed to be the reflection of the control
        point on the previous command relative to the current point.
        (If there is no previous command or if the previous command was
        not a quadratic Bezier curve command such as Q, q, T or t, assume
        the control point is coincident with the current point.)

        SVG equivalent: "t <dx> <dy>"
        )docstring",
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cPathFigure.def(
        "addCubicBezierCurveTo",
        &omnetpp::cPathFigure::addCubicBezierCurveTo,
        R"docstring(
        Appends a CubicBezierCurveTo item to the path. It will draw a cubic
        Bezier curve from the current point to (x,y) using (x1,y1) as the
        control point at the beginning of the curve and (x2,y2) as the
        control point at the end of the curve.

        SVG equivalent: "C <x1> <y1> <x2> <y2> <x> <y>"
        )docstring",
        pybind11::arg("x1"), pybind11::arg("y1"),
        pybind11::arg("x2"), pybind11::arg("y2"),
        pybind11::arg("x"), pybind11::arg("y")
    );

    py_cPathFigure.def(
        "addCubicBezierCurveRel",
        &omnetpp::cPathFigure::addCubicBezierCurveRel,
        R"docstring(
        Appends a CubicBezierCurveRel item to the path. It will draw a cubic
        Bezier curve from the current point (lastx, lasty) to (lastx+dx, lasty+dy)
        using (lastx+dx1, lasty+dy1) as the control point at the beginning
        of the curve and (lastx+dx2, lasty+dy2) as the control point at the
        end of the curve.

        SVG equivalent: "c <dx1> <dy1> <dx2> <dy2> <dx> <dy>"
        )docstring",
        pybind11::arg("dx1"), pybind11::arg("dy1"),
        pybind11::arg("dx2"), pybind11::arg("dy2"),
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cPathFigure.def(
        "addSmoothCubicBezierCurveTo",
        &omnetpp::cPathFigure::addSmoothCubicBezierCurveTo,
        R"docstring(
        Appends a SmoothCubicBezierCurveTo item to the path. It will draw
        a cubic Bezier curve from the current point to (x,y).
        The first control point is assumed to be the reflection of the
        second control point on the previous command relative to the last
        point. (If there is no previous command or if the previous command was
        not a cubic Bezier command such as C, c, S or s, assume the first
        control point is coincident with the current point.) (x2,y2) is the
        second control point (i.e., the control point at the end of the curve).

        SVG equivalent: "S <x2> <y2> <x> <y>"
        )docstring",
        pybind11::arg("x2"), pybind11::arg("y2"),
        pybind11::arg("x"), pybind11::arg("y")
    );

    py_cPathFigure.def(
        "addSmoothCubicBezierCurveRel",
        &omnetpp::cPathFigure::addSmoothCubicBezierCurveRel,
        R"docstring(
        Appends a SmoothCubicBezierCurveRel item to the path. It will draw
        a cubic Bezier curve from the current point (lastx, lasty) to (lastx+dx,
        ty+dy). The first control point is assumed to be the reflection
        of the second control point on the previous command relative to the last
        point. (If there is no previous command or if the previous command was
        not a cubic Bezier command such as C, c, S or s, assume the first
        control point is coincident with the current point.) (lastx+dx2, lasty+dy2)
        is the second control point (i.e., the control point at the end of the curve).

        SVG equivalent: "s <dx2> <dy2> <dx> <dy>"
        )docstring",
        pybind11::arg("dx2"), pybind11::arg("dy2"),
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cPathFigure.def(
        "addClosePath",
        &omnetpp::cPathFigure::addClosePath,
        R"docstring(
        Appends a ClosePath item to the path. It will close the current subpath
        by drawing a straight line from the current point to current subpath's
        initial point.

        SVG equivalent: "Z" or "z"
        )docstring"
    );
}

void bind_cAbstractTextFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cAbstractTextFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cAbstractTextFigure, pybind11::nodelete>
    > py_cAbstractTextFigure(
        m,
        "_cAbstractTextFigure",
        R"docstring(
        Abstract base class for figures that display text. Text may be multi-line.

        The location of the text on the canvas is determined jointly by the
        <i>position</i> and <i>anchor</i> properties. The anchor tells how to
        position the text relative to the positioning point. For example,
        if anchor is ANCHOR_CENTER then the text is centered on the point;
        if anchor is ANCHOR_N then the text will be drawn so that its top center
        is at the positioning point. The values ANCHOR_BASELINE_START,
        ANCHOR_BASELINE_MIDDLE, ANCHOR_BASELINE_END refer to the beginning,
        middle and end of the baseline of the (first line of the) text as
        anchor point. Anchor defaults to ANCHOR_CENTER.

        Other properties in this class define the font, color and opacity of the text.
        )docstring"
    );

    py_cAbstractTextFigure.def(
        "dup",
        &omnetpp::cAbstractTextFigure::dup
    );

    py_cAbstractTextFigure.def(
        "__str__",
        &omnetpp::cAbstractTextFigure::str
    );

    py_cAbstractTextFigure.def(
        "str",
        &omnetpp::cAbstractTextFigure::str
    );

    py_cAbstractTextFigure.def(
        "parse",
        &omnetpp::cAbstractTextFigure::parse
    );

    py_cAbstractTextFigure.def(
        "moveLocal",
        &omnetpp::cAbstractTextFigure::moveLocal,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cAbstractTextFigure.def(
        "getPosition",
        &omnetpp::cAbstractTextFigure::getPosition,
        R"docstring(
        Returns the position of the text. The position and the anchoring
        mode jointly determine the placement of the text item; see the
        class description for details.
        )docstring"
    );

    py_cAbstractTextFigure.def(
        "setPosition",
        &omnetpp::cAbstractTextFigure::setPosition,
        R"docstring(
        Sets the position of the text. The position and the anchoring
        mode jointly determine the placement of the text item; see the
        class description for details.
        )docstring",
        pybind11::arg("position")
    );

    py_cAbstractTextFigure.def(
        "getAnchor",
        &omnetpp::cAbstractTextFigure::getAnchor,
        R"docstring(
        Returns the anchoring mode of the text. The position and the
        anchoring mode jointly determine the placement of the text item;
        see the class description for details.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractTextFigure.def(
        "setAnchor",
        &omnetpp::cAbstractTextFigure::setAnchor,
        R"docstring(
        Sets the anchoring mode of the text. The position and the anchoring
        mode jointly determine the placement of the text item; see the
        class description for details.
        )docstring",
        pybind11::arg("anchor")
    );

    py_cAbstractTextFigure.def(
        "getAnchor",
        &omnetpp::cAbstractTextFigure::getAnchor,
        R"docstring(
        Returns the anchoring mode of the text. The position and the
        anchoring mode jointly determine the placement of the text item;
        see the class description for details.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractTextFigure.def(
        "getBounds",
        &omnetpp::cAbstractTextFigure::getBounds,
        R"docstring(
        Returns the bounding box of the text figure.

        Caveat: Note that the figure itself only has a position and an anchor
        available for computing the bounding box, size (and for some cases
        ascent height) need to be supplied by the user interface of the
        simulation (cEnvir). The result depends on the details of font
        rendering and other factors, so the returned numbers and their
        accuracy may vary across user interfaces (Qtenv vs. Tkenv), operating
        systems or even installations. Cmdenv and other non-GUI environments
        may return completely made-up (but not entirely unrealistic) numbers.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractTextFigure.def(
        "getColor",
        &omnetpp::cAbstractTextFigure::getColor,
        R"docstring(
        Returns the text color.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractTextFigure.def(
        "setColor",
        &omnetpp::cAbstractTextFigure::setColor,
        R"docstring(
        Sets the text color. The default color is black.
        )docstring",
        pybind11::arg("color")
    );

    py_cAbstractTextFigure.def(
        "getOpacity",
        &omnetpp::cAbstractTextFigure::getOpacity,
        R"docstring(
        Returns the opacity of the figure, a number in the [0,1] interval.
        )docstring"
    );

    py_cAbstractTextFigure.def(
        "setOpacity",
        &omnetpp::cAbstractTextFigure::setOpacity,
        R"docstring(
        Sets the line opacity of the figure. The argument must be a number
        in the [0,1] interval. The default opacity is 1.0.
        )docstring",
        pybind11::arg("opacity")
    );

    py_cAbstractTextFigure.def(
        "getHalo",
        &omnetpp::cAbstractTextFigure::getHalo,
        R"docstring(
        Returns true if a "halo" is displayed around the text.
        )docstring"
    );

    py_cAbstractTextFigure.def(
        "setHalo",
        &omnetpp::cAbstractTextFigure::setHalo,
        R"docstring(
        Sets the flag that controls whether a partially transparent "halo"
        should be displayed around the text. The halo improves the
        readability of the text when it is displayed over a background
        that has similar color as the text, or when it overlaps with other
        text items. The default setting is false (no halo).

        Note: in Qtenv, halo may be slow to render for text figures
        (cTextFigure) under some circumstances. There is no such penalty
        for label figures (cLabelFigure).
        )docstring",
        pybind11::arg("opacity")
    );

    py_cAbstractTextFigure.def(
        "getFont",
        &omnetpp::cAbstractTextFigure::getFont,
        R"docstring(
        Returns the font used for the figure.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractTextFigure.def(
        "setFont",
        &omnetpp::cAbstractTextFigure::setFont,
        R"docstring(
        Sets the font to be used for the figure. The default font has no
        specific typeface or size set, allowing the UI to use its preferred
        font.
        )docstring",
        pybind11::arg("opacity")
    );

    py_cAbstractTextFigure.def(
        "getText",
        &omnetpp::cAbstractTextFigure::getText,
        R"docstring(
        Returns the text to be displayed.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractTextFigure.def(
        "setText",
        &omnetpp::cAbstractTextFigure::setText,
        R"docstring(
        Sets the text to be displayed. The text may contain newline and tab
        characters ("\n", "\t") for formatting.
        )docstring",
        pybind11::arg("opacity")
    );
}


void bind_cTextFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cTextFigure, omnetpp::cAbstractTextFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cTextFigure, pybind11::nodelete>
    > py_cTextFigure(
        m,
        "_cTextFigure",
        R"docstring(
        A figure that displays text which is affected by zooming and transformations.

        Font, color, position, anchoring and other properties are inherited from
        cAbstractTextFigure.
        )docstring"
    );

    py_cTextFigure.def(
        pybind11::init<const char *>(),
        pybind11::arg("name") = nullptr
    );

    py_cTextFigure.def(
        "dup",
        &omnetpp::cTextFigure::dup
    );

    py_cTextFigure.def(
        "__str__",
        &omnetpp::cTextFigure::str
    );

    py_cTextFigure.def(
        "str",
        &omnetpp::cTextFigure::str
    );
}

void bind_cLabelFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cLabelFigure, omnetpp::cAbstractTextFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cLabelFigure, pybind11::nodelete>
    > py_cLabelFigure(
        m,
        "_cLabelFigure",
        R"docstring(
        A figure that displays text which is unaffected by zooming or
        transformations, except for its position.

        Font, color, position, anchoring and other properties are inherited from
        cAbstractTextFigure.
        )docstring"
    );

    py_cLabelFigure.def(
        pybind11::init<const char *>(),
        pybind11::arg("name") = nullptr
    );

    py_cLabelFigure.def(
        "dup",
        &omnetpp::cLabelFigure::dup
    );
}

void bind_cAbstractImageFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cAbstractImageFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cAbstractImageFigure, pybind11::nodelete>
    > py_cAbstractImageFigure(
        m,
        "_cAbstractImageFigure",
        R"docstring(
        Abstract base class for figures that display an image.

        The location of the image on the canvas is determined jointly by the
        <i>position</i> and <i>anchor</i> properties. The anchor tells how to
        position the image relative to the positioning point. For example,
        if anchor is ANCHOR_CENTER then the image is centered on the point;
        if anchor is ANCHOR_N then the image will be drawn so that its top center
        point is at the positioning point. Anchor defaults to ANCHOR_CENTER.

        Images may be drawn at their "natural" size, or may be scaled to a
        specified size by setting the width and/or height properties. One can
        choose from several interpolation modes that control how the image is
        rendered. Interpolation defaults to INTERPOLATION_FAST.

        Images can be tinted; this feature is controlled by a tint color and
        a tint amount, a [0,1] real number.

        Images may be rendered as partially transparent, which is controlled
        by the opacity property, a [0,1] real number. (The rendering process
        will combine this property with the transparency information contained
        in the image, i.e. the alpha channel.)
        )docstring"
    );

    py_cAbstractImageFigure.def(
        "dup",
        &omnetpp::cAbstractImageFigure::dup
    );

    py_cAbstractImageFigure.def(
        "parse",
        &omnetpp::cAbstractImageFigure::parse
    );

    py_cAbstractImageFigure.def(
        "moveLocal",
        &omnetpp::cAbstractImageFigure::moveLocal,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cAbstractImageFigure.def(
        "getPosition",
        &omnetpp::cAbstractImageFigure::getPosition,
        R"docstring(
        Returns the position of the image. The position and the anchoring
        mode jointly determine the placement of the image; see the class
        description for details.
        )docstring"
    );

    py_cAbstractImageFigure.def(
        "setPosition",
        &omnetpp::cAbstractImageFigure::setPosition,
        R"docstring(
        Sets the position of the image. The position and the anchoring
        mode jointly determine the placement of the image; see the
        class description for details.
        )docstring",
        pybind11::arg("position")
    );

    py_cAbstractImageFigure.def(
        "getAnchor",
        &omnetpp::cAbstractImageFigure::getAnchor,
        R"docstring(
        Returns the anchoring mode of the image. The position and the
        anchoring mode jointly determine the placement of the image;
        see the class description for details.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractImageFigure.def(
        "setAnchor",
        &omnetpp::cAbstractImageFigure::setAnchor,
        R"docstring(
        Sets the anchoring mode of the image. The position and the
        anchoring mode jointly determine the placement of the image;
        see the class description for details.
        )docstring",
        pybind11::arg("anchor")
    );

    py_cAbstractImageFigure.def(
        "getWidth",
        &omnetpp::cAbstractImageFigure::getWidth,
        R"docstring(
        Returns zero if the image is displayed with its "natural" width,
        or the width (a positive number) if it is scaled.
        )docstring"
    );

    py_cAbstractImageFigure.def(
        "setWidth",
        &omnetpp::cAbstractImageFigure::setWidth,
        R"docstring(
        Sets the width of the image figure. Use this method if you want
        to display the image at a size different from its "natural" size.
        To clear this setting, specify 0 as parameter. The default value
        is 0 (natural width).
        )docstring",
        pybind11::arg("width")
    );

    py_cAbstractImageFigure.def(
        "getHeight",
        &omnetpp::cAbstractImageFigure::getHeight,
        R"docstring(
        Returns zero if the image is displayed with its "natural" height,
        or the height (a positive number) if it is scaled.
        )docstring"
    );

    py_cAbstractImageFigure.def(
        "setHeight",
        &omnetpp::cAbstractImageFigure::setHeight,
        R"docstring(
        Sets the height of the image figure. Use this method if you want
        to display the image at a size different from its "natural" size.
        To clear this setting, specify 0 as parameter. The default value
        is 0 (natural height).
        )docstring",
        pybind11::arg("height")
    );

    py_cAbstractImageFigure.def(
        "setSize",
        &omnetpp::cAbstractImageFigure::setSize,
        R"docstring(
        Sets the image figure's width and height together. Delegates to
        setWidth() and setHeight().
        )docstring",
        pybind11::arg("width"), pybind11::arg("height")
    );

    py_cAbstractImageFigure.def(
        "getBounds",
        &omnetpp::cAbstractImageFigure::getBounds,
        R"docstring(
        returns the image's bounds in the current coordinate system.

        caveat: for cimagefigure/ciconfigure, when no explicit size is
        specified for the image using setwidth()/setheight() and the
        simulation is running under cmdenv or another non-gui environment,
        the image size used for the bounds computation may be a guess or
        a made-up size, as cmdenv does not actually load the images files.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractImageFigure.def(
        "getInterpolation",
        &omnetpp::cAbstractImageFigure::getInterpolation,
        R"docstring(
        Returns the interpolation mode for the image. The interpolation mode
        determines how the image is scaled to the size it is rendered with
        on the screen.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractImageFigure.def(
        "setInterpolation",
        &omnetpp::cAbstractImageFigure::setInterpolation,
        R"docstring(
        Sets the interpolation mode for the image. The interpolation mode
        determines how the image is scaled to the size it is rendered with
        on the screen. The default interpolation mode is "fast".
        )docstring",
        pybind11::arg("interpolation")
    );

    py_cAbstractImageFigure.def(
        "getOpacity",
        &omnetpp::cAbstractImageFigure::getOpacity,
        R"docstring(
        Returns the opacity of the figure.
        )docstring"
    );

    py_cAbstractImageFigure.def(
        "setOpacity",
        &omnetpp::cAbstractImageFigure::setOpacity,
        R"docstring(
        Sets the opacity of the figure. The argument must be a number
        in the [0,1] interval. The default opacity is 1.0.
        )docstring",
        pybind11::arg("opacity")
    );

    py_cAbstractImageFigure.def(
        "getTintColor",
        &omnetpp::cAbstractImageFigure::getTintColor,
        R"docstring(
        Returns the tint color of the figure.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cAbstractImageFigure.def(
        "setTintColor",
        &omnetpp::cAbstractImageFigure::setTintColor,
        R"docstring(
        Sets the tint color of the figure. The tint color and the tint
        amount jointly determine the colorization of the figure.
        The default tint color is blue.
        )docstring",
        pybind11::arg("tintColor")
    );

    py_cAbstractImageFigure.def(
        "getTintAmount",
        &omnetpp::cAbstractImageFigure::getTintAmount,
        R"docstring(
        Returns the tint amount of the figure, a number in the [0,1]
        interval.
        )docstring"
    );

    py_cAbstractImageFigure.def(
        "setTintAmount",
        &omnetpp::cAbstractImageFigure::setTintAmount,
        R"docstring(
        Sets the tint amount of the figure. The argument must be a number
        in the [0,1] interval. The tint color and the tint amount jointly
        determine the colorization of the figure. The default tint amount
        is 0.
        )docstring",
        pybind11::arg("tintAmount")
    );
}

void bind_cImageFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cImageFigure, omnetpp::cAbstractImageFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cImageFigure, pybind11::nodelete>
    > py_cImageFigure(
        m,
        "_cImageFigure",
        R"docstring(
        A figure that displays an image, typically an icon or a background image,
        loaded from the \opp image path.

        Positioning and other properties are inherited from cAbstractImageFigure.
        )docstring"
    );

    py_cImageFigure.def(
        pybind11::init<const char *>(),
        pybind11::arg("name") = nullptr
    );

    py_cImageFigure.def(
        "dup",
        &omnetpp::cImageFigure::dup
    );

    py_cImageFigure.def(
        "parse",
        &omnetpp::cImageFigure::parse
    );

    py_cImageFigure.def(
        "moveLocal",
        &omnetpp::cImageFigure::moveLocal,
        pybind11::arg("dx"), pybind11::arg("dy")
    );

    py_cImageFigure.def(
        "__str__",
        &omnetpp::cImageFigure::str
    );

    py_cImageFigure.def(
        "str",
        &omnetpp::cImageFigure::str
    );

    py_cImageFigure.def(
        "getImageName",
        &omnetpp::cImageFigure::getImageName,
        R"docstring(
        Returns the string that identifies the image to display.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cImageFigure.def(
        "setImageName",
        &omnetpp::cImageFigure::setImageName,
        R"docstring(
        Sets the image to display. The imageName parameter must identify an
        image loaded from the \opp image path, in the form also used by the
        display string "i" tag: with subdirectory names but without filename
        extension. Examples: "block/queue" or "device/switch_vl".
        )docstring",
        pybind11::arg("name")
    );

    py_cImageFigure.def(
        "getImageNaturalWidth",
        &omnetpp::cImageFigure::getImageNaturalWidth,
        R"docstring(
        Returns the "natural" width of the image. For raster images, this is
        the width of the image in pixels.

        Caveat: when the simulation is running under Cmdenv or another
        non-GUI environment, the returned width may be a guess or a made-up
        number, as Cmdenv does not actually load the images files.
        )docstring"
    );

    py_cImageFigure.def(
        "getImageNaturalHeight",
        &omnetpp::cImageFigure::getImageNaturalHeight,
        R"docstring(
        Returns the "natural" height of the image. For raster images, this is
        the height of the image in pixels.

        Caveat: when the simulation is running under Cmdenv or another
        non-GUI environment, the returned height may be a guess or a made-up
        number, as Cmdenv does not actually load the images files.
        )docstring"
    );
}

void bind_cIconFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cIconFigure, omnetpp::cImageFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cIconFigure, pybind11::nodelete>
    > py_cIconFigure(
        m,
        "_cIconFigure",
        R"docstring(
        A figure that displays an image, typically an icon or a background image,
        loaded from the \opp image path, in a non-zooming way.

        Positioning and other properties are inherited from cAbstractImageFigure.

        Note: The image is not affected by transforms or zoom, except for its position.
        )docstring"
    );

    py_cIconFigure.def(
        pybind11::init<const char *>(),
        pybind11::arg("name") = nullptr
    );

    py_cIconFigure.def(
        "dup",
        &omnetpp::cIconFigure::dup
    );
}

void bind_cPixmapFigure(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cPixmapFigure, omnetpp::cAbstractImageFigure, omnetpp::cFigure,
        std::unique_ptr<omnetpp::cPixmapFigure, pybind11::nodelete>
    > py_cPixmapFigure(
        m,
        "_cPixmapFigure",
        R"docstring(
        A figure that displays an image that can be manipulated programmatically.

        A pixmap figure may be used to display e.g. a heat map. Support for scaling
        and various interpolation modes are useful here.
        )docstring"
    );

    py_cPixmapFigure.def(
        pybind11::init<const char *>(),
        pybind11::arg("name") = nullptr
    );

    py_cPixmapFigure.def(
        "dup",
        &omnetpp::cPixmapFigure::dup
    );

    py_cPixmapFigure.def(
        "parse",
        &omnetpp::cPixmapFigure::parse
    );

    py_cPixmapFigure.def(
        "__str__",
        &omnetpp::cPixmapFigure::str
    );

    py_cPixmapFigure.def(
        "str",
        &omnetpp::cPixmapFigure::str
    );

    py_cPixmapFigure.def(
        "getPixmap",
        &omnetpp::cPixmapFigure::getPixmap,
        R"docstring(
        Returns the displayed pixmap. It must not be modified directly (e.g.
        after casting away the const modifier), because the GUI would not be
        notified about the change. Use the pixel manipulation methods of the
        figure class, or make a copy of the pixmap, update it, and then set it
        back on the figure using setPixmap().
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cPixmapFigure.def(
        "setPixmap",
        &omnetpp::cPixmapFigure::setPixmap,
        R"docstring(
        Sets the pixmap to be displayed. Note that this involves copying
        the pixmap data.
        )docstring",
        pybind11::arg("pixmap")
    );

    py_cPixmapFigure.def(
        "getPixmapWidth",
        &omnetpp::cPixmapFigure::getPixmapWidth,
        R"docstring(
        Delegates to the similar method of the contained Pixmap instance.
        )docstring"
    );

    py_cPixmapFigure.def(
        "getPixmapHeight",
        &omnetpp::cPixmapFigure::getPixmapHeight,
        R"docstring(
        Delegates to the similar method of the contained Pixmap instance.
        )docstring"
    );

    py_cPixmapFigure.def(
        "setPixmapSize",
        pybind11::overload_cast<int, int, const omnetpp::cFigure::RGBA &>(&omnetpp::cPixmapFigure::setPixmapSize),
        R"docstring(
        Delegates to the similar method of the contained Pixmap instance.

        Nondestructive, set *newly added* pixels with this color and opacity.
        )docstring",
        pybind11::arg("width"), pybind11::arg("height"), pybind11::arg("fill")
    );

    py_cPixmapFigure.def(
        "setPixmapSize",
        pybind11::overload_cast<int, int, const omnetpp::cFigure::Color&, double>(&omnetpp::cPixmapFigure::setPixmapSize),
        R"docstring(
        Delegates to the similar method of the contained Pixmap instance.

        Nondestructive, set *newly added* pixels with this color and opacity.
        )docstring",
        pybind11::arg("width"), pybind11::arg("height"), pybind11::arg("color"), pybind11::arg("opacity")
    );

    py_cPixmapFigure.def(
        "fillPixmap",
        pybind11::overload_cast<const omnetpp::cFigure::RGBA &>(&omnetpp::cPixmapFigure::fillPixmap),
        R"docstring(
        Delegates to the similar method of the contained Pixmap instance.
        )docstring",
        pybind11::arg("fill")
    );

    py_cPixmapFigure.def(
        "fillPixmap",
        pybind11::overload_cast<const omnetpp::cFigure::Color&, double>(&omnetpp::cPixmapFigure::fillPixmap),
        R"docstring(
        Delegates to the similar method of the contained Pixmap instance.
        )docstring",
        pybind11::arg("color"), pybind11::arg("opacity")
    );

    py_cPixmapFigure.def(
        "getPixel",
        &omnetpp::cPixmapFigure::getPixel,
        R"docstring(
        Delegates to the similar method of the contained Pixmap instance.
        )docstring",
        pybind11::arg("x"), pybind11::arg("y"),
        pybind11::return_value_policy::reference
    );

    py_cPixmapFigure.def(
        "setPixel",
        pybind11::overload_cast<int, int, const omnetpp::cFigure::RGBA&>(&omnetpp::cPixmapFigure::setPixel),
        R"docstring(
        Delegates to the similar method of the contained Pixmap instance.
        )docstring",
        pybind11::arg("x"), pybind11::arg("y"), pybind11::arg("argb"),
        pybind11::return_value_policy::reference
    );

    py_cPixmapFigure.def(
        "setPixel",
        pybind11::overload_cast<int, int, const omnetpp::cFigure::Color &, double>(&omnetpp::cPixmapFigure::setPixel),
        R"docstring(
        Delegates to the similar method of the contained Pixmap instance.
        )docstring",
        pybind11::arg("x"), pybind11::arg("y"), pybind11::arg("color"), pybind11::arg("opacity") = 1.0,
        pybind11::return_value_policy::reference
    );

    py_cPixmapFigure.def(
        "getPixelColor",
        &omnetpp::cPixmapFigure::getPixelColor,
        R"docstring(
        Delegates to the similar method of the contained Pixmap instance.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cPixmapFigure.def(
        "setPixelColor",
        &omnetpp::cPixmapFigure::setPixelColor,
        R"docstring(
        Delegates to the similar method of the contained Pixmap instance.
        )docstring",
        pybind11::arg("x"), pybind11::arg("y"), pybind11::arg("color")
    );

    py_cPixmapFigure.def(
        "getPixelOpacity",
        &omnetpp::cPixmapFigure::getPixelOpacity,
        R"docstring(
        Delegates to the similar method of the contained Pixmap instance.
        )docstring"
    );

    py_cPixmapFigure.def(
        "setPixelOpacity",
        &omnetpp::cPixmapFigure::setPixelOpacity,
        R"docstring(
        Delegates to the similar method of the contained Pixmap instance.
        )docstring",
        pybind11::arg("x"), pybind11::arg("y"), pybind11::arg("opacity")
    );
}
