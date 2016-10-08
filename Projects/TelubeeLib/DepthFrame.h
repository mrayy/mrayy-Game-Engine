

/********************************************************************
	created:	2014/02/17
	created:	17:2:2014   1:10
	filename: 	C:\Development\mrayEngine\Projects\TelubeeLib\DepthFrame.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeLib
	file base:	DepthFrame
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __DepthFrame__
#define __DepthFrame__




namespace mray
{
namespace TBee
{
	class DepthFrameImpl;
class DepthFrame
{
public:
	template<class T>
	class FrameData
	{
	protected:
		T* m_data;
		math::vector2di m_size;
	public:

		FrameData()
		{
			m_data = 0;
		}
		~FrameData()
		{
			clean();
		}

		T* Data(){ return m_data; }
		const T* Data()const { return m_data; }
		const math::vector2di& Size()const{ return m_size; }
		void clean()
		{
			delete[]m_data;
			m_data = 0;
		}

		void createData(int w, int h)
		{
			if (w != m_size.x || h != m_size.y)
			{
				delete[] m_data;
				m_size.x = w;
				m_size.y = h;
				m_data = new T[w*h];
			}
		}

		void copyData(const T* ptr, int w, int h)
		{
			createData(w, h);
			memcpy(m_data, ptr, w*h*sizeof(T));
		}
	};
protected:
	DepthFrameImpl *m_impl;
	friend class DepthFrameImpl;
public:
	DepthFrame();
	virtual~DepthFrame();

	void Clean();

	void EnableThreshold(bool e);
	bool IsThresholdEnabled();
	void SetThresholds(float min, float max);

	float GetMinThreshold();
	float GetMaxThreshold();

	void SetNormalAmplitude(float amp);
	float GetNormalAmplitude();

	void EnableSmoothing(bool e);
	bool IsSmoothingEnabled();

	void EnableGapFill(bool e);
	bool IsGapFillEnabled();
	void SetBandThreshold(int inner, int outer);
	int GetInnerBandThreshold();
	int GetOuterBandThreshold();


	const math::vector2di& GetSize()const;
	float* GetDepth();
	math::vector3d* GetNormals();

	void CalculateDepth();
	void CalculateNormals();

	void CreateRawData(int w, int h);
	void SetRawData(ushort* d, int w, int h);
	ushort* GetRawData();
	const ushort* GetRawData()const;
	uint GetRawDataLength()const;

	math::recti  GetCroppedFrame(const math::recti& src, const DepthFrame* f);
	void AddFrame(const math::recti& src, const  DepthFrame* f);
};

}
}


#endif
