#ifndef SIC_EXTERNAL_H_
#define SIC_EXTERNAL_H_

namespace sic {

/**
 * An object that is defined in an external system.
 */
class External {

public:
	/// For objects defined in an external system, an ID for that system.
	typedef unsigned long ID;

private:
	sic::External::ID id;

protected:
	/**
	 * Initialise the external details of an object.
	 *
	 * This is protected to ensure it is only inherited as part of another
	 * class.
	 */
	External(sic::External::ID id) : id(id) {}

public:
	/**
	 * Get the object's external ID representation.
	 */
	sic::External::ID getExternalID() const { return id; }
};

} // namespace sic

#endif // SIC_EXTERNAL_H_
