#ifndef __CTHEAD_H__
#define __CTHEAD_H__

#include "Common.h"

//////////////////////////////////////////////////////////////////////////////////

class CThread {
	public:
												CThread (std::string name="");
		virtual									~CThread ();

		void									start ();
		void									cancel ();
		void                            		join ();

	protected:
		static void*                    		entryPoint (void* ctx);
		virtual void                    		main () = 0;

		std::string								m_name;
		int										m_id;
		pthread_t								m_threadId;
		bool									m_started;
		bool									m_canceled;
		bool									m_joined;
};

#endif
