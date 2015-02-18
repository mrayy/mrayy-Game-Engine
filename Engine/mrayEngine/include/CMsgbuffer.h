


#ifndef ___CMsgBuffer___
#define ___CMsgBuffer___

#include <mTypes.h>
#include <IStream.h>

#include <assert.h>

namespace mray{	
namespace network{



class CMsgBuffer:mray::OS::IStream
{
	byte *m_data;
	int m_used;
	int m_alloc;
	bool m_freeWhenDestroy;

	int pos;
protected:
	virtual uint calcSizeInternal(){
		return m_alloc;
	}

public:
	CMsgBuffer():IStream(mT("")){
		m_freeWhenDestroy=true;
		m_data=0;
		m_used=m_alloc=0;
		pos=0;
	}
	CMsgBuffer(uint size):IStream(mT("")){
		m_freeWhenDestroy=true;
		m_data=0;
		m_used=m_alloc=0;
		resize(size);
		pos=0;
	}

	virtual bool canRead(){
		return pos<m_used;
	}
	virtual bool canSeek(){
		return true;
	}
	virtual bool canWrite(){
		return true;
	}

	virtual~CMsgBuffer(){
		clear();
	}

	byte& operator [](uint index)
	{
		assert(index<m_used); // access violation

		return m_data[index];
	}

	const byte& operator [](uint index)const
	{
		assert(index<m_used); // access violation

		return m_data[index];
	}


	void increaseUsed(uint size,bool copy=true){
		if(m_alloc>m_used+size){
			m_used+=size;
		}else
		{
			realloc(m_used+size,copy);
			m_used=m_used+size;
		}
	}

	void realloc(int new_size,bool copy=true){
		if(new_size<0)
			new_size=0;
		void* old_data =0;
		if(m_data)
			old_data= m_data;

		m_data =new byte[new_size];
		m_alloc = new_size;

		if(old_data){
			if(copy){
				// copy old data
				int end = m_used < new_size ? m_used : new_size;
				memcpy(m_data,old_data,end*sizeof(byte));
			}
			//try{
			delete [] old_data;
		/*	}catch(...){
			}*/
		}
		if (m_alloc < m_used)
			m_used = m_alloc;
	}

	void push_back(const byte element)
	{
		if (m_used + 1 > m_alloc)
		{
			byte e;
			e=element;
			realloc(m_used * 2 +1); // increase data block

			m_data[m_used++]= e; 

			return;
		}

		m_data[m_used++] = element;
	}

	void clear()
	{
		if(m_freeWhenDestroy){
			try{
				delete [] m_data;
			}catch(...){
			}
			m_data = 0;
			m_used = 0;
			m_alloc = 0;
		}
	}

	void setFreeWhenDestroy(bool s){
		m_freeWhenDestroy=s;
	}
	bool isFreeWhenDestroy(){
		return m_freeWhenDestroy;
	}

	void resize(int size){
		if(size<0)
			size=0;
		if(size<=m_alloc)
			m_used=size;
		else{
			realloc(size);
			m_used=size;
		}
		if(pos>m_used)pos=m_used;
	}

	void set_pointer(byte* newPointer, uint size,bool FreeWhenDestroy=true)
	{
		clear();
		m_freeWhenDestroy=FreeWhenDestroy;

		m_data = newPointer;
		m_alloc = size;
		m_used = size;
	}
	byte* pointer()
	{
		return m_data;
	}

	const byte* const_pointer() const
	{
		return m_data;
	}

	uint size() const
	{
		return m_used;
	}
	uint left() const
	{
		return m_used-pos;
	}

	uint allocated_size() const
	{
		return m_alloc;
	}

	bool empty() const
	{
		return m_used == 0;
	}





	void reset(){
		seek(0,OS::ESeek_Set);
	}

	
	virtual bool isOpen(){
		return true;
	}
	virtual int read(void*data,size_t size){
		int cnt=0;
		if(pos+size>m_used){
			cnt=m_used-pos;
		}else
			cnt=size;
		memcpy(data,m_data+pos,cnt);

		pos+=cnt;
		return cnt;
	}
	virtual int write(const void*data,size_t size){
		if(pos+size>m_used)
			resize(pos+size);
		memcpy(m_data+pos,data,size);
		pos+=size;

		return size;
	}
	virtual int seek(int bytes,mray::OS::EStreamSeek mode){
		switch(mode){
			case mray::OS::ESeek_Cur:
				pos+=bytes;
				break;
			case mray::OS::ESeek_End:
				pos=m_used-bytes;
				break;
			case mray::OS::ESeek_Set:
				pos=bytes;
				break;
		}
		if(pos<=0)pos=0;
		if(pos>m_used)pos=m_used;
		return pos;
	}
	virtual int getPos(){
		return pos;
	}
	virtual void putChar(int c){
		write(&c,sizeof(c));
	}
	virtual int getChar(){
		char out=0;
		read(&out,sizeof(out));
		return out;
	}
	virtual int getInt(){
		int out=0;
		read(&out,sizeof(out));
		return out;
	}
	virtual float getFloat(){
		float out=0;
		read(&out,sizeof(out));
		return out;
	}
	virtual void putInt(int v){
		write(&v,sizeof(v));
	}
	virtual void putFloat(float v){
		write(&v,sizeof(v));
	}
	//! get size of file
	virtual int getSize(){
		return size();
	}
	virtual bool eof(){
		return pos>=m_used;
	}
	virtual void close(){
	}
	virtual int getString(mray::core::string&out,int max){
		if(pos+max>=m_used)max=m_used-pos-1;

		char *tmp=new char[max];
		memcpy(tmp,m_data+pos,max);
		out=tmp;
		pos+=max;
		try{
			delete [] tmp;
		}catch(...){
		}
		return max;
	}

	virtual int readLine(char*out)
	{
		return 0;
	}
	virtual int print(const mchar*text)
	{
		return 0;
	}
};


}
}


#endif



