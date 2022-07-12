#ifndef __VSNC_SIP_CLIENT_H_
#define __VSNC_SIP_CLIENT_H_
#include "structurer.h"

struct eXosip_t;

namespace vsnc
{
	namespace sip
	{
		class SIPClient
		{
		public:
			SIPClient(SIPParam sipParam);
			~SIPClient();

			void Close() noexcept;
			bool Reister() noexcept;
			bool Refresh() noexcept;
			bool Logout(const std::string reason) noexcept;
			
			bool Invite(const std::string sdp) noexcept;
			bool Bye() noexcept;
			bool Message(const std::string context) noexcept;
			bool Notify(const std::string context) noexcept;
			
			bool Subscription(const SubscriptionParam subParam) noexcept;
		private:
			eXosip_t*    m_pExcontent;
			SIPHeader*   m_pFrom;
			SIPHeader*   m_pTo;
			SIPAuthInfo* m_pSIPAuth;
			int          m_iRegisterID;
			int          m_iCallID;
			int          m_iDialogID;

		};
	}
}

#endif // !__VSCM_SIP_CLIENT_H_

