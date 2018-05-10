#ifndef SIC_MODEL_H_
#define SIC_MODEL_H_

#include "sic/External.hh"

namespace sic {

class Model : public sic::External {

public:
	Model(sic::External::ID externalID) : sic::External(externalID){};
};

} // namespace sic

#endif // SIC_MODEL_H_
