#include "datalink.h"

/**
 * @brief Updates the shared credit limit values based on the received DLLP information
 * @param flit Flit containing the DLLPs
 * @param P_SHARED_CREDIT_LIMIT Array of shared credit limit for posted traffic
 * @param NP_SHARED_CREDIT_LIMIT Array of shared credit limit for non-posted traffic
 * @param CPL_SHARED_CREDIT_LIMIT Array of shared credit limit for completion traffic
 * @param FI1 First indication (FI1) flag for credit update
 * @param FI2 Second indication (FI2) flag for credit update
*/
void DataLinkLayer::updateCreditLimit(Flit flit, int(&P_SHARED_CREDIT_LIMIT)[2], int(&NP_SHARED_CREDIT_LIMIT)[2], int(&CPL_SHARED_CREDIT_LIMIT)[2], bool& FI1, bool& FI2) {
	// Static variables to store the flags and state across function calls all initialized to false
	static bool discard, pFc2Flag, npFc2Flag, cplFc2Flag;

	// The flit contains a DLLP, with size of 32 bit from 14th byte
	auto dllpPayload = std::bitset<32>((flit.dllpPayload.to_ulong() >> (14 * 8)) & 0xffffffff);
	auto dllpObj = Dllp::DllpObjRep(dllpPayload);

	// Discard received DLLPs if the discard flag is set
	if (discard) {
		switch (dllpObj.m_type)
		{
		case Dllp::DllpType::initFC2:
			switch (dllpObj.m_creditType)
			{
			case Dllp::CreditType::Cpl:
				cplFc2Flag = true;
				break;
			case Dllp::CreditType::P:
				pFc2Flag = true;
				break;
			case Dllp::CreditType::NP:
				npFc2Flag = true;
				break;
			default:
				break;
			}
			break;
		}
	}

	// Update credit limits if the discard flag is not set and the FI1 flag is not set
	if (!discard && !FI1)
	{
		switch (dllpObj.m_creditType)
		{
		case Dllp::CreditType::Cpl:

			break;
		case Dllp::CreditType::P:
			P_SHARED_CREDIT_LIMIT[0] = dllpObj.HdrFC;
			P_SHARED_CREDIT_LIMIT[1] = dllpObj.DataFc;
			break;
		case Dllp::CreditType::NP:
			NP_SHARED_CREDIT_LIMIT[0] = dllpObj.HdrFC;
			NP_SHARED_CREDIT_LIMIT[1] = dllpObj.DataFc;
			break;
		default:
			break;
		}
	}
	// Set the FI1 flag if all credit limits are updated
	if (P_SHARED_CREDIT_LIMIT[0] != -1 && NP_SHARED_CREDIT_LIMIT[0] != -1 && CPL_SHARED_CREDIT_LIMIT[0] != -1) {
		FI1 = true;
		// Set the FI2 flag if all expected DLLPs are received
		if (!FI2 && pFc2Flag && npFc2Flag && cplFc2Flag)
			FI2 = true;
	}
}