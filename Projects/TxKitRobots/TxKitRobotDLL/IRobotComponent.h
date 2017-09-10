

#ifndef __IROBOTCOMPONENT__
#define __IROBOTCOMPONENT__

namespace mray
{
	
class IRobotComponent
{
protected:

public:
	IRobotComponent(){}
	virtual ~IRobotComponent(){}

	virtual std::string GetType() = 0;

	virtual bool Connect(const core::string& port,bool autoSearchPort=true) = 0;
	virtual bool IsConnected() = 0;
	virtual void Disconnect() = 0;

	virtual void Start() {}
	virtual void Stop() {}
	virtual bool IsStarted(){ return true; }

	virtual std::string ExecCommand(const core::string& cmd, const core::string& args) { return ""; }
};

}


#endif
