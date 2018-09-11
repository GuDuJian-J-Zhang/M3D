#include "m3d/model/IShape.h"
namespace M3D
{
	SVLGlobalID SVLGlobalID::G_DefaultGlobalID;

	bool SVLGlobalID::operator<(const SVLGlobalID &cmp) const
	{
		bool cmpState = this->Code() < cmp.Code();
		return cmpState;
	}

	SVLGlobalID SVLGlobalID::operator + (const SVLGlobalID &cmp) const
	{
		return SVLGlobalID(this->documentId + cmp.documentId, this->objectId + cmp.objectId);
	}

	SVLGlobalID& SVLGlobalID::operator += (const SVLGlobalID &cmp)
	{
		this->documentId += cmp.documentId;
		this->objectId += cmp.objectId;
		return *this;
	}

	SVLGlobalID SVLGlobalID::operator- (const SVLGlobalID &cmp) const
	{
		return SVLGlobalID(this->documentId - cmp.documentId, this->objectId - cmp.objectId);
	}

	SVLGlobalID& SVLGlobalID::operator -= (const SVLGlobalID &cmp)
	{
		this->documentId -= cmp.documentId;
		this->objectId -= cmp.objectId;
		return *this;
	}

	SVLGlobalID::SVLGlobalID(unsigned int documentId, unsigned int objectId)
	{
		this->documentId = documentId;
		this->objectId = objectId;
	}

	bool SVLGlobalID::operator ==(const SVLGlobalID &cmp)
	{
		bool cmpState = (this->documentId == cmp.documentId && this->objectId == cmp.objectId);
		return cmpState;
	}

	bool SVLGlobalID::operator !=(const SVLGlobalID &cmp)
	{
		bool cmpState = !(*this == cmp);
		return cmpState;
	}


	SVLGlobalID::SVLGlobalID()
	{
		this->documentId = 0;
		this->objectId = 0;
	}

	SVLGlobalID::SVLGlobalID(unsigned int objectId)
	{
		this->documentId = 0;
		this->objectId = objectId;
	}
}
