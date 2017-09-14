#ifndef OPENBEAGLE_TRUSSEVALOP_H
#define OPENBEAGLE_TRUSSEVALOP_H

#include "Beagle/GP.hpp"
#include "TrussConfig.hpp"

class TrussEvalOp : public Beagle::GP::EvaluationOp {
public:
	typedef Beagle::AllocatorT<TrussEvalOp, Beagle::EvaluationOp::Alloc> Alloc;
	typedef Beagle::PointerT<TrussEvalOp, Beagle::EvaluationOp::Handle> Handle;
	typedef Beagle::ContainerT<TrussEvalOp, Beagle::EvaluationOp::Bag> Bag;

	explicit TrussEvalOp();

	virtual Beagle::Fitness::Handle evaluate(Beagle::Individual &individual, Beagle::Context &context);
	virtual void init(Beagle::System &system);
	virtual void registerParams(Beagle::System &system);

protected:

	TrussConfig::Handle config;
	Beagle::String::Handle string_config;
};

#endif //OPENBEAGLE_TRUSSEVALOP_H
