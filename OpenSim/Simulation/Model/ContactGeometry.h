#ifndef OPENSIM_CONTACT_GEOMETRY_H_
#define OPENSIM_CONTACT_GEOMETRY_H_ 
/* -------------------------------------------------------------------------- *
 *                        OpenSim:  ContactGeometry.h                         *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2016 Stanford University and the Authors                *
 * Author(s): Peter Eastman                                                   *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */
// INCLUDE
#include <OpenSim/Simulation/osimSimulationDLL.h>
#include "OpenSim/Common/Object.h"
#include "OpenSim/Simulation/SimbodyEngine/Body.h"
#include "OpenSim/Simulation/Model/ModelComponent.h"
#include <SimTKsimbody.h>

namespace OpenSim {

class ScaleSet;

/** This class represents the physical shape of an object for use in contact
 * modeling.  It is an abstract class, with subclasses for particular geometric
 * representations. The geometry is attached to a PhysicalFrame, which is
 * specified using a Connector name "frame".
 *
 * @author Peter Eastman
 */
class OSIMSIMULATION_API ContactGeometry : public ModelComponent {
OpenSim_DECLARE_ABSTRACT_OBJECT(ContactGeometry, ModelComponent);

public:

//=============================================================================
// PROPERTIES
//=============================================================================

    OpenSim_DECLARE_PROPERTY(location, SimTK::Vec3,
        "Location of geometry center in the PhysicalFrame.");

    OpenSim_DECLARE_PROPERTY(orientation, SimTK::Vec3,
        "Orientation of geometry in the PhysicalFrame.");

    OpenSim_DECLARE_PROPERTY(display_preference, int,
        "0:Hide 1:Wire 3:Flat 4:Shaded");

    OpenSim_DECLARE_LIST_PROPERTY_SIZE(color, double, 3,
        "Display Color to apply to the contact geometry.");

//=============================================================================
// METHODS
//=============================================================================
public:
    // CONSTRUCTION
    /** Construct an empty ContactGeometry. */
    ContactGeometry();

    /** This constructor connects this ContactGeometry to the provided `frame`,
     * and uses the default location and orientation (both `Vec3(0)`).
     *
     * @param frame        the PhysicalFrame this geometry is attached to;
     */
    ContactGeometry(const PhysicalFrame& frame);

    /**
     * @param location     the location of the geometry expressed in `frame`
     * @param orientation  the orientation of the geometry expressed in `frame`
     *                     as XYZ body-fixed Euler angles.
     * @param frame        the PhysicalFrame this geometry is attached to;
     *                     this constructor connects this ContactGeometry to
     *                     the provided `frame`
     */
    ContactGeometry(const SimTK::Vec3& location,
                    const SimTK::Vec3& orientation,
                    const PhysicalFrame& frame);


    // ACCESSORS
#ifndef SWIG
    /** Get the PhysicalFrame this geometry is attached to. */
    const PhysicalFrame& getFrame() const;
#endif
    /** %Set the PhysicalFrame this geometry is attached to. */
    void setFrame(const PhysicalFrame& body);
    /** Get the path name of the PhysicalFrame this geometry is attached to. */
    const std::string& getFrameName() const;
    /** %Set the path name (relative or absolute) of the PhysicalFrame this
     * geometry is attached to. */
    void setFrameName(const std::string& name);

    /** Get the display_preference of this geometry. */
    const int getDisplayPreference();
    /** %Set the display_preference of this geometry. */
    void setDisplayPreference(const int dispPref);

    /** Create a new SimTK::ContactGeometry based on this object. */
    virtual SimTK::ContactGeometry createSimTKContactGeometry() = 0;

    /** Get a Transform representing the position and orientation of the
     * geometry within the Body (or base frame) it is attached to (*not* the
     * "frame" that the geometry is connected to). If `B` is the base or Body
     * frame, `F` is the frame that this geometry is connected to, and `P` is
     * the (imaginary) frame that is defined (relative to `F`) by the location
     * and orientation properties of this class, then this returns
     * `X_BF * X_FP`. */
    SimTK::Transform findTransformInBaseFrame() const;

    /**
    * Scale a ContactGeometry based on XYZ scale factors for the bodies.
    * 
    * @param aScaleSet Set of XYZ scale factors for the bodies.
    */
    virtual void scale(const ScaleSet& aScaleSet);

    // Override this method if geometry changes/deforms
    virtual void updateGeometry() {};

    /** @name Deprecated */
    // @{
    /** <b>(Deprecated)</b> Use get_location() instead. */
    DEPRECATED_14("use get_location() instead")
    const SimTK::Vec3& getLocation() const;

    /** <b>(Deprecated)</b> Use set_location() instead. */
    DEPRECATED_14("use set_location() instead")
    void setLocation(const SimTK::Vec3& location);

    /** <b>(Deprecated)</b> Use get_orientation() instead. */
    DEPRECATED_14("use get_orientation() instead")
    const SimTK::Vec3& getOrientation() const;

    /** <b>(Deprecated)</b> Use set_orientation() instead. */
    DEPRECATED_14("use set_orientation() instead")
    void setOrientation(const SimTK::Vec3& orientation);

#ifndef SWIG
    /** <b>(Deprecated)</b> Use getFrame() instead.
     * Get the Body this geometry is attached to. */
    DEPRECATED_14("use getFrame() instead")
    const PhysicalFrame& getBody() const;
#endif

    /** <b>(Deprecated)</b> Use setFrame() instead.
     * %Set the Body this geometry is attached to. */
    DEPRECATED_14("use setFrame() instead")
    void setBody(const PhysicalFrame& body);

    /** <b>(Deprecated)</b> Use getFrameName() instead.
     * Get the name of the Body this geometry is attached to. */
    DEPRECATED_14("use getFrameName() instead")
    const std::string& getBodyName() const;

    /** <b>(Deprecated)</b> Use setFrameName() instead.
     * %Set the name of the Body this geometry is attached to. */
    DEPRECATED_14("use setFrameName() instead")
    void setBodyName(const std::string& name);

    /** <b>(Deprecated)</b> Use findTransformInBaseFrame() instead, which does
     * the same thing.
     *
     * Get a Transform representing the position and orientation of the
     * geometry within the Body (or base frame) it is attached to (*not* the
     * "frame" that the geometry is connected to). If `B` is the base or Body
     * frame, `F` is the frame that this geometry is connected to, and `P` is
     * the (imaginary) frame that is defined (relative to `F`) by the location
     * and orientation properties of this class, then this returns
     * `X_BF * X_FP`. */
    DEPRECATED_14("use findTransformInBaseFrame() instead")
    SimTK::Transform getTransform() const;
    // @}

protected:

    void updateFromXMLNode(SimTK::Xml::Element& node, int versionNumber)
        override;

private:
    // INITIALIZATION
    void setNull();
    void constructProperties() override;
    void constructConnectors() override;

//=============================================================================
// DATA
//=============================================================================

protected:

//=============================================================================
};  // END of class ContactGeometry
//=============================================================================
//=============================================================================

} // end of namespace OpenSim

#endif // OPENSIM_CONTACT_GEOMETRY_H_ 
