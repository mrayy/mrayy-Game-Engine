

#ifndef ___SVIDEO_MODE___
#define ___SVIDEO_MODE___





namespace mray{
namespace video{

struct VMode
{
	math::Point2di dim;
	int bits;
	VMode():dim(0),bits(0)
	{}
	VMode(math::Point2di Dim,int Bits):dim(Dim),bits(Bits)
	{}
	bool operator ==(VMode&other)
	{
		return (dim==other.dim && bits==other.bits);
	}
};
class SVideoModes
{

	std::vector<VMode> modes;
	VMode desktop;
public:
	~SVideoModes(){modes.clear();}
	void addMode(math::Point2di Dim,int Bits)
	{
		modes.push_back(VMode(Dim,Bits));
	}
	int getCount(){return modes.size();}
	std::vector<VMode>& getModes(){return modes;}

	bool isModeEnumerated(VMode mode)
	{
		for(uint i=0;i<modes.size();++i)
		{
			if(modes[i]==mode)return 1;
		}
		return 0;
	}

	math::Point2di getDesktopSize(){return desktop.dim;}
	int getDesktopBits(){return desktop.bits;}
	void setDesktopMode(math::Point2di dim,int bits)
	{
		desktop.dim=dim;
		desktop.bits=bits;
	}
	VMode getMode(int index){
		if(index <0 || index >= modes.size())return VMode();
		return modes[index];
	}
};

}
}

#endif









