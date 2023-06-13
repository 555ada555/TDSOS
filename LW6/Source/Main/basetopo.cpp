
#pragma warning (disable : 4127)

#include "definitions.h"
#include "basetopo.h"

//=============================================================================
#pragma code_seg("PAGE")
CMiniportTopologySimpleAudioSample::CMiniportTopologySimpleAudioSample
(
    _In_        PCFILTER_DESCRIPTOR    *FilterDesc,
    _In_        USHORT                  DeviceMaxChannels
)

{
    PAGED_CODE();

    DPF_ENTER(("[%s]",__FUNCTION__));

    m_AdapterCommon     = NULL;

    ASSERT(FilterDesc != NULL);
    m_FilterDescriptor  = FilterDesc;
    m_PortEvents        = NULL;
    
    ASSERT(DeviceMaxChannels > 0);
    m_DeviceMaxChannels = DeviceMaxChannels;
} // CMiniportTopologySimpleAudioSample

CMiniportTopologySimpleAudioSample::~CMiniportTopologySimpleAudioSample
(
    void
)
/*++

Routine Description:

  Topology miniport destructor

Arguments:

Return Value:

  void

--*/
{
    PAGED_CODE();

    DPF_ENTER(("[%s]",__FUNCTION__));

    SAFE_RELEASE(m_AdapterCommon);
    SAFE_RELEASE(m_PortEvents);
} // ~CMiniportTopologySimpleAudioSample

//=============================================================================
#pragma code_seg("PAGE")
NTSTATUS
CMiniportTopologySimpleAudioSample::DataRangeIntersection
( 
    _In_  ULONG                 PinId,
    _In_  PKSDATARANGE          ClientDataRange,
    _In_  PKSDATARANGE          MyDataRange,
    _In_  ULONG                 OutputBufferLength,
    _Out_writes_bytes_to_opt_(OutputBufferLength, *ResultantFormatLength)
          PVOID                 ResultantFormat     OPTIONAL,
    _Out_ PULONG                ResultantFormatLength 
)

{
    UNREFERENCED_PARAMETER(PinId);
    UNREFERENCED_PARAMETER(ClientDataRange);
    UNREFERENCED_PARAMETER(MyDataRange);
    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(ResultantFormat);
    UNREFERENCED_PARAMETER(ResultantFormatLength);

    PAGED_CODE();

    DPF_ENTER(("[%s]",__FUNCTION__));

    return (STATUS_NOT_IMPLEMENTED);
} // DataRangeIntersection

//=============================================================================
#pragma code_seg("PAGE")
NTSTATUS
CMiniportTopologySimpleAudioSample::GetDescription
( 
    _Out_ PPCFILTER_DESCRIPTOR *  OutFilterDescriptor 
)

{
    PAGED_CODE();

    ASSERT(OutFilterDescriptor);

    DPF_ENTER(("[%s]",__FUNCTION__));

    *OutFilterDescriptor = m_FilterDescriptor;

    return (STATUS_SUCCESS);
} // GetDescription

//=============================================================================
#pragma code_seg("PAGE")
NTSTATUS
CMiniportTopologySimpleAudioSample::Init
( 
    _In_  PUNKNOWN          UnknownAdapter_,
    _In_  PPORTTOPOLOGY     Port_ 
)

{
    PAGED_CODE();
    
    ASSERT(UnknownAdapter_);
    ASSERT(Port_);

    DPF_ENTER(("[CMiniportTopologySimpleAudioSample::Init]"));

    NTSTATUS    ntStatus;

    ntStatus = 
        UnknownAdapter_->QueryInterface( 
            IID_IAdapterCommon,
            (PVOID *) &m_AdapterCommon);
    
    if (NT_SUCCESS(ntStatus))
    {
        //
        // Get the port event interface.
        //
        ntStatus = Port_->QueryInterface(
            IID_IPortEvents, 
            (PVOID *)&m_PortEvents);
    }

    if (NT_SUCCESS(ntStatus))
    {
        m_AdapterCommon->MixerReset();
    }

    if (!NT_SUCCESS(ntStatus))
    {
        // clean up AdapterCommon
        SAFE_RELEASE(m_AdapterCommon);
        SAFE_RELEASE(m_PortEvents);
    }

    return ntStatus;
} // Init

#pragma code_seg("PAGE")
NTSTATUS                            
CMiniportTopologySimpleAudioSample::PropertyHandlerGeneric
(
    _In_  PPCPROPERTY_REQUEST     PropertyRequest
)

{
    PAGED_CODE();

    NTSTATUS                    ntStatus = STATUS_INVALID_DEVICE_REQUEST;

    switch (PropertyRequest->PropertyItem->Id)
    {
        case KSPROPERTY_AUDIO_VOLUMELEVEL:
            ntStatus = PropertyHandler_Volume(
                                m_AdapterCommon,
                                PropertyRequest,
                                m_DeviceMaxChannels);
            break;
        
        case KSPROPERTY_AUDIO_MUTE:
            ntStatus = PropertyHandler_Mute(
                                m_AdapterCommon,
                                PropertyRequest,
                                m_DeviceMaxChannels);
            break;

        case KSPROPERTY_AUDIO_PEAKMETER2:
            ntStatus = PropertyHandler_PeakMeter2(
                                m_AdapterCommon,
                                PropertyRequest,
                                m_DeviceMaxChannels);
            break;

        case KSPROPERTY_AUDIO_CPU_RESOURCES:
            ntStatus = PropertyHandler_CpuResources(PropertyRequest);
            break;

        case KSPROPERTY_AUDIO_MUX_SOURCE:
            ntStatus = PropertyHandlerMuxSource(PropertyRequest);
            break;

        case KSPROPERTY_AUDIO_DEV_SPECIFIC:
            ntStatus = PropertyHandlerDevSpecific(PropertyRequest);
            break;

        default:
            DPF(D_TERSE, ("[PropertyHandlerGeneric: Invalid Device Request]"));
    }

    return ntStatus;
} 
#pragma code_seg("PAGE")
NTSTATUS                            
CMiniportTopologySimpleAudioSample::PropertyHandlerMuxSource
(
    _In_  PPCPROPERTY_REQUEST     PropertyRequest
)
{
    PAGED_CODE();

    DPF_ENTER(("[%s]",__FUNCTION__));

    NTSTATUS                    ntStatus = STATUS_INVALID_DEVICE_REQUEST;
    {
        if (PropertyRequest->ValueSize >= sizeof(ULONG))
        {
            PULONG pulMuxValue = PULONG(PropertyRequest->Value);
            
            if (PropertyRequest->Verb & KSPROPERTY_TYPE_GET)
            {
                *pulMuxValue = m_AdapterCommon->MixerMuxRead();
                PropertyRequest->ValueSize = sizeof(ULONG);
                ntStatus = STATUS_SUCCESS;
            }
            else if (PropertyRequest->Verb & KSPROPERTY_TYPE_SET)
            {
                m_AdapterCommon->MixerMuxWrite(*pulMuxValue);
                ntStatus = STATUS_SUCCESS;
            }
            else if (PropertyRequest->Verb & KSPROPERTY_TYPE_BASICSUPPORT)
            {
                ntStatus = 
                    PropertyHandler_BasicSupport
                    ( 
                        PropertyRequest, 
                        KSPROPERTY_TYPE_ALL,
                        VT_I4
                    );
            }
        }
        else
        {
            DPF(D_TERSE, ("[PropertyHandlerMuxSource - Invalid parameter]"));
            ntStatus = STATUS_INVALID_PARAMETER;
        }
    }

    return ntStatus;
} 
#pragma code_seg("PAGE")
NTSTATUS                            
CMiniportTopologySimpleAudioSample::PropertyHandlerDevSpecific(
    _In_  PPCPROPERTY_REQUEST     PropertyRequest
)
{
    PAGED_CODE();

    DPF_ENTER(("[%s]",__FUNCTION__));

    NTSTATUS ntStatus=STATUS_SUCCESS;

    if (PropertyRequest->Verb & KSPROPERTY_TYPE_BASICSUPPORT)
    {
        if( DEV_SPECIFIC_VT_BOOL == PropertyRequest->Node )
        {
            ntStatus = PropertyHandler_BasicSupport(PropertyRequest,KSPROPERTY_TYPE_ALL,VT_BOOL);
        }
        else
        {
            ULONG ExpectedSize = sizeof( KSPROPERTY_DESCRIPTION ) + 
                                 sizeof( KSPROPERTY_MEMBERSHEADER ) + 
                                 sizeof( KSPROPERTY_BOUNDS_LONG );
            DWORD ulPropTypeSetId;

            if( DEV_SPECIFIC_VT_I4 == PropertyRequest->Node )
            {
                ulPropTypeSetId = VT_I4;
            }
            else if ( DEV_SPECIFIC_VT_UI4 == PropertyRequest->Node )
            {
                ulPropTypeSetId = VT_UI4;
            }
            else
            {
                ulPropTypeSetId = VT_ILLEGAL;
                ntStatus = STATUS_INVALID_PARAMETER;
            }

            if( NT_SUCCESS(ntStatus))
            {
                if ( !PropertyRequest->ValueSize )
                {
                    PropertyRequest->ValueSize = ExpectedSize;
                    ntStatus = STATUS_BUFFER_OVERFLOW;
                } 
                else if (PropertyRequest->ValueSize >= sizeof(KSPROPERTY_DESCRIPTION))
                {
                    
                    PKSPROPERTY_DESCRIPTION PropDesc = PKSPROPERTY_DESCRIPTION(PropertyRequest->Value);

                    PropDesc->AccessFlags       = KSPROPERTY_TYPE_ALL;
                    PropDesc->DescriptionSize   = ExpectedSize;
                    PropDesc->PropTypeSet.Set   = KSPROPTYPESETID_General;
                    PropDesc->PropTypeSet.Id    = ulPropTypeSetId;
                    PropDesc->PropTypeSet.Flags = 0;
                    PropDesc->MembersListCount  = 0;
                    PropDesc->Reserved          = 0;

                    if ( PropertyRequest->ValueSize >= ExpectedSize )
                    {
                    
                        PropDesc->MembersListCount  = 1;

                        PKSPROPERTY_MEMBERSHEADER MembersHeader = ( PKSPROPERTY_MEMBERSHEADER )( PropDesc + 1);
                        MembersHeader->MembersFlags = KSPROPERTY_MEMBER_RANGES;
                        MembersHeader->MembersCount  = 1;
                        MembersHeader->MembersSize   = sizeof( KSPROPERTY_BOUNDS_LONG );
                        MembersHeader->Flags = 0;

                        PKSPROPERTY_BOUNDS_LONG PeakMeterBounds = (PKSPROPERTY_BOUNDS_LONG)( MembersHeader + 1);
                        if(VT_I4 == ulPropTypeSetId )
                        {
                            PeakMeterBounds->SignedMinimum = 0;
                            PeakMeterBounds->SignedMaximum = 0x7fffffff;
                        }
                        else
                        {
                            PeakMeterBounds->UnsignedMinimum = 0;
                            PeakMeterBounds->UnsignedMaximum = 0xffffffff;
                        }

                        // set the return value size
                        PropertyRequest->ValueSize = ExpectedSize;
                    }
                    else
                    {
                        // No extra information to return.
                        PropertyRequest->ValueSize = sizeof(KSPROPERTY_DESCRIPTION);
                    }

                    ntStatus = STATUS_SUCCESS;
                } 
                else if (PropertyRequest->ValueSize >= sizeof(ULONG))
                {
                    // if return buffer can hold a ULONG, return the access flags
                    //
                    *(PULONG(PropertyRequest->Value)) = KSPROPERTY_TYPE_ALL;

                    PropertyRequest->ValueSize = sizeof(ULONG);
                    ntStatus = STATUS_SUCCESS;                    
                }
                else
                {
                    PropertyRequest->ValueSize = 0;
                    ntStatus = STATUS_BUFFER_TOO_SMALL;
                }
            }
        }
    }
    else
    {
        switch( PropertyRequest->Node )
        {
        case DEV_SPECIFIC_VT_BOOL:
            {
                PBOOL pbDevSpecific;

                ntStatus = ValidatePropertyParams(PropertyRequest, sizeof(BOOL), 0);

                if (NT_SUCCESS(ntStatus))
                {
                    pbDevSpecific   = PBOOL (PropertyRequest->Value);

                    if (PropertyRequest->Verb & KSPROPERTY_TYPE_GET)
                    {
                        *pbDevSpecific = m_AdapterCommon->bDevSpecificRead();
                        PropertyRequest->ValueSize = sizeof(BOOL);
                    }
                    else if (PropertyRequest->Verb & KSPROPERTY_TYPE_SET)
                    {
                        m_AdapterCommon->bDevSpecificWrite(*pbDevSpecific);
                    }
                    else
                    {
                        ntStatus = STATUS_INVALID_PARAMETER;
                    }
                }
            }
            break;
        case DEV_SPECIFIC_VT_I4:
            {
                INT* piDevSpecific;

                ntStatus = ValidatePropertyParams(PropertyRequest, sizeof(int), 0);

                if (NT_SUCCESS(ntStatus))
                {
                    piDevSpecific   = PINT (PropertyRequest->Value);

                    if (PropertyRequest->Verb & KSPROPERTY_TYPE_GET)
                    {
                        *piDevSpecific = m_AdapterCommon->iDevSpecificRead();
                        PropertyRequest->ValueSize = sizeof(int);
                    }
                    else if (PropertyRequest->Verb & KSPROPERTY_TYPE_SET)
                    {
                        m_AdapterCommon->iDevSpecificWrite(*piDevSpecific);
                    }
                    else
                    {
                        ntStatus = STATUS_INVALID_PARAMETER;
                    }
                }
            }
            break;
        case DEV_SPECIFIC_VT_UI4:
            {
                UINT* puiDevSpecific;

                ntStatus = ValidatePropertyParams(PropertyRequest, sizeof(UINT), 0);

                if (NT_SUCCESS(ntStatus))
                {
                    puiDevSpecific   = PUINT (PropertyRequest->Value);

                    if (PropertyRequest->Verb & KSPROPERTY_TYPE_GET)
                    {
                        *puiDevSpecific = m_AdapterCommon->uiDevSpecificRead();
                        PropertyRequest->ValueSize = sizeof(UINT);
                    }
                    else if (PropertyRequest->Verb & KSPROPERTY_TYPE_SET)
                    {
                        m_AdapterCommon->uiDevSpecificWrite(*puiDevSpecific);
                    }
                    else
                    {
                        ntStatus = STATUS_INVALID_PARAMETER;
                    }
                }
            }
            break;
        default:
            ntStatus = STATUS_INVALID_PARAMETER;
            break;
        }


        if( !NT_SUCCESS(ntStatus))
        {
            DPF(D_TERSE, ("[%s - ntStatus=0x%08x]",__FUNCTION__,ntStatus));
        }
    }

    return ntStatus;
} 
#pragma code_seg("PAGE")
VOID
CMiniportTopologySimpleAudioSample::AddEventToEventList
(
    _In_  PKSEVENT_ENTRY    EventEntry 
)
/*++

Routine Description:

  The AddEventToEventList method adds an event to the port driver's event list

Arguments:

  EventEntry - 

--*/
{
    PAGED_CODE();
    DPF_ENTER(("[CMiniportTopology::AddEventToEventList]"));

    ASSERT(m_PortEvents != NULL);

    m_PortEvents->AddEventToEventList(EventEntry);
}

//=============================================================================
#pragma code_seg()
VOID
CMiniportTopologySimpleAudioSample::GenerateEventList
(
    _In_opt_    GUID   *Set,
    _In_        ULONG   EventId,
    _In_        BOOL    PinEvent,
    _In_        ULONG   PinId,
    _In_        BOOL    NodeEvent,
    _In_        ULONG   NodeId
)

{
    DPF_ENTER(("[CMiniportTopologySimpleAudioSample::GenerateEventList]"));

    ASSERT(m_PortEvents != NULL);

    m_PortEvents->GenerateEventList(
        Set,
        EventId,
        PinEvent,
        PinId,
        NodeEvent,
        NodeId);
}
 
#pragma code_seg()
