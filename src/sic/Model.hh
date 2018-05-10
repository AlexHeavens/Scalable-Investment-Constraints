#ifndef SIC_MODEL_H_
#define SIC_MODEL_H_

#include "sic/External.hh"

namespace sic {

class Model : sic::External {

public:
	Model(sic::External externalID) : sic::External(externalID){};
};

} // namespace sic

#endif // SIC_MODEL_H_
