/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Universitat Autònoma de Barcelona
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Rubén Martínez <rmartinez@deic.uab.cat>
 */

#ifndef LTP_HEADER_H
#define LTP_HEADER_H

#include "ns3/header.h"
#include "ns3/trailer.h"
#include "ns3/address.h"
#include <vector>

namespace ns3 {

//namespace ltp {

/**
 * \enum SegmentType
 * \brief LTP Segment Types
 */
enum SegmentType
{
    SINGLE = 111,
    MULTIPLE = 1
  /*LTPTYPE_RD  = 0,                      //!< Red Data Segment
  LTPTYPE_RD_CP  = 1,                   //!< Red Data Checkpoint Segment
  LTPTYPE_RD_CP_EORP  = 2,              //!< Red Data End of Red Part Segment
  LTPTYPE_RD_CP_EORP_EOB = 3,           //!< Red Data End of Red Part End of Block Segment
  LTPTYPE_GD = 4,                       //!< Green Data Segment
  LTPTYPE_GD_UF1 = 5,                   //!< Green Data Undefined Segment
  LTPTYPE_GD_UF2 = 6,                   //!< Green Data Undefined Segment
  LTPTYPE_GD_EOB = 7,                   //!< Green Data End of Block Segment
  LTPTYPE_RS = 8,                       //!< Report Segment
  LTPTYPE_RAS = 9,                      //!< Report ACK Segment
  LTPTYPE_CTRL1 = 10,                   //!< Control Segment Undefined
  LTPTYPE_CTRL2 = 11,                   //!< Control Segment Undefined
  LTPTYPE_CS = 12,                      //!< Cancel Segment (sent by Sender)
  LTPTYPE_CAS = 13,                     //!< Cancel ACK Segment (reply to Sender)
  LTPTYPE_CR = 14,                      //!< Cancel Segment (sent by Receiver)
  LTPTYPE_CAR = 15,                     //!< Cancel ACK Segment (reply to Receiver)*/
};

/**
 * \enum CxReasonCode
 * \brief LTP Cancelation Reason Code
 */
enum CxReasonCode
{
  USR_CNCLD = 0,                       //!<  Client Service Canceled
  UNREACH = 1,                         //!<  Unreachable Client Service
  RLEXC = 2,                           //!<  Retransmit Limit Exceeded
  MISCOLORED = 3,                      //!<  RS with a GS offset or the opposite case
  SYS_CNCLD = 4,                       //!<  System Error Unexpect Termination
  RXMTCYCEXC = 5,                      //!<  Exceeded Retransmision Cycles Limit
  RESERVED = 6,                        //!<  06-FF reserved.

};

/**
 * \ingroup dtn
 *
 * \brief Session ID Class for LTP Segment headers:
 * A session Id uniquely identifies, among all transmissions, the session
 * to which a segment pertains.
 */
class SessionId
{
public:
  /**
   * \brief Construct a null session ID
   */
  SessionId ();
  ~SessionId ();
  /**
   * \brief Construct a session ID from specified parameters.
   * \param originator Session Originator
   * \param value Session Number
   */
  SessionId (uint64_t originator,uint64_t value);

  /**
   * \returns the session originator of this session ID.
   */
  uint64_t GetSessionOriginator () const;
  /**
   * \brief get session number
   * \returns the session number of this session ID
   */
  uint64_t GetSessionNumber () const;

  /**
   * \brief set session originator.
   * \param originator session originator.
   */
  void SetSessionOriginator (uint64_t originator);
  /**
   * \brief set session number.
   * \param value session number.
   */
  void SetSessionNumber (uint64_t value);

  bool operator == (SessionId const &o) const;
  bool operator < (SessionId const &o) const;

  /**
   *  CCSDS 734.1-R-2: 3.4 -
   *  Session numbers chosen by LTP senders shall be in the range [1, 2^32-1]
   */
  static const uint32_t MIN_SESSION_NUMBER = 1;
  static const uint32_t MAX_SESSION_NUMBER = 4294967295U;

private:

  uint64_t m_sessionOriginator;  //!< The Ltp Engine ID of the sender  (SDNV encoded)
  uint64_t m_sessionNumber;      //!< Random number generated by the sender.  (SDNV encoded)

};

std::ostream& operator << (std::ostream& os, const SessionId& pvf);

/**
 * \brief Class for LTP Extensions as defined in RFC 5327.
 * This class defines the main structure of an LTP extension.
 * Extensions can be found in both the header and trailer of an LTP segment,
 * they are stored using a type-length-value (TLV) representation.
 */
class LtpExtension
{

public:
  /**
  * \enum ExtensionType
  * \brief LTP Extension Tag , indicates the type of extension
  */
  enum ExtensionType
  {
    LTPEXT_AUTH = 0,   //!< LTP authentication extension
    LTPEXT_COOKIE = 1  //!< LTP cookie extension
  };

  uint32_t GetSerializedSize (void) const;

  uint32_t Deserialize (Buffer::Iterator &start);
  void Serialize (Buffer::Iterator &start) const;

  /**
  * \returns Extension tag code
  */
  ExtensionType GetExtensionType () const;
  /**
  * \returns Extension length
  */
  uint64_t GetExtensionLength () const;

  /**
  * \param type Extension tag code.
  */
  void SetExtensionType (ExtensionType type);
  /**
  * \brief Adds new data to the extension data and increases the length counter.
  * \param data new byte of data
  */
  void AddExtensionData (uint8_t data);
  /**
  * \brief Removes all data from the extension.
  */
  void ClearExtensionData ();

  bool operator == (LtpExtension const &o) const;

private:
  uint8_t m_extType;             //!< Extension Tag
  uint64_t m_len;                //!< Extension Length (SDNV encoded)
  std::vector<uint8_t> m_value;  //!< Extension data

};

/**
 * \ingroup dtn
 *
 * \brief LTP Segment Header: Defined in RFC-5326 section 3.1
  \verbatim

  0     1     2     3     4     5     6     7     8
  +-----+-----+-----+-----+-----+-----+-----+-----+
  |                 Smart Meter ID                | 
  |                                               |
  +-----+-----+-----+-----+-----+-----+-----+-----+
  |                 Fragment Type                 |
  +-----+-----+-----+-----+-----+-----+-----+-----+
  |                  Fragment ID                  |
  +-----+-----+-----+-----+-----+-----+-----+-----+
  |                 Fragment Size                 |
  |                                               |
  +-----+-----+-----+-----+-----+-----+-----+-----+

  \endverbatim
*/
class LtpHeader : public Header
{

public:
  /**
  * \brief Construct a null LTP segment
  */
  LtpHeader ();
  /**
  * \brief Construct a LTP segment from given parameters
  * \param segmentType Type of segment.
  * \param sId Session Id.
  */
  LtpHeader (SegmentType segmentType, SessionId sId);
  ~LtpHeader ();


  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Serialize (Buffer::Iterator start) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Print (std::ostream &os) const;

  /* Setter Methods */

  /**
  * \param version Protocol version.
  */
  void SetVersion (uint8_t version);
  /**
  * \param segmentType Segmentype enum value.
  */
  void SetSegmentType (SegmentType segmentType);
  /**
  * \param count Number of header extensions.
  */
  void SetHeaderExtensionCount (uint8_t count);
  /**
  * \param count Number of trailer extensions.
  */
  void SetTrailerExtensionCount (uint8_t count);
  /**
  * \param id SessionId object.
  */
  void SetSessionId (SessionId id);
  
////////////////////////////////////////////////////////////////////////////////

void SetSmartMeterID(uint16_t SM_ID);

uint16_t GetSmartMeterID() const;

void SetFragmentType(u_int8_t fragmentType);

uint8_t GetFragmentType() const;

void SetFragmentID(uint8_t fragmentID);

uint8_t GetFragmentID() const;

void SetFragmentSize(uint16_t fragmentSize);

uint16_t GetFragmentSize() const;

////////////////////////////////////////////////////////////////////////////////  
  
  /**
  * \param extension Extension object.
  */
  void AddExtension (LtpExtension extension);


  /* Getter Methods */
  /**
   * \return Protocol version.
   */
  uint8_t GetVersion () const;
  /**
  * \return Segmentype enum value.
  */
  SegmentType GetSegmentType () const;
  /**
   * \return Number of header extensions.
   */
  uint8_t GetHeaderExtensionCount () const;
  /**
   * \return Number of trailer extensions.
   */
  uint8_t GetTrailerExtensionCount () const;
  /**
   * \return SessionId object
   */
  SessionId GetSessionId () const;
  /**
   * \return Extesion object
   * \param index Requested index
   */
  LtpExtension GetExtension (uint32_t index) const;

  /* Static packet type checkers */

  /*
   * \brief Check if this type corresponds to a data segment
   * \param type segment type.
   * \return true if data segment, false otherwise.
   */
  static bool IsDataSegment (SegmentType type);
  /*
   * \brief Check if this type corresponds to a red data segment
   * \param type segment type.
   * \return true if data segment, false otherwise.
   */
  static bool IsRedDataSegment (SegmentType type);

  /*
   * \brief Check if this type corresponds to a red data segment
   * \param type segment type.
   * \return true if data segment, false otherwise.
   */
  static bool IsGreenDataSegment (SegmentType type);

  bool operator == (LtpHeader const &o) const;

private:
  // Control byte
  uint8_t m_version;              //!< Protocol Version (4 MSBs)
  uint8_t m_typeFlags;            //!< Segment Type Flag (4 LSBs)
  
  uint16_t m_smartMeterID;
  uint8_t m_fragmentType;
  uint8_t m_fragmentID;
  uint16_t m_fragmentSize;

  // SessionId n-bytes
  SessionId m_sessionId;          //!< SessionID (SDNV encoded =  8 bit multiple with no fixed size).

  // Extension Count Byte
  uint8_t m_hdrExtensionCnt;      //!< Header Extension Count (4 MSBs)
  uint8_t m_trailerExtensionCnt;  //!< Trailer Extension Count (4 LSBs)

  std::vector<LtpExtension> m_extensions;     //!< Extensions in TLV format (from 0 to 15)

};

/**
 * \ingroup dtn
 *
 * \brief Class for LTP Trailer as defined in RFC-5326 section 3.3
 */
class LtpTrailer : public Trailer
{
public:
  LtpTrailer ();
  ~LtpTrailer ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Serialize (Buffer::Iterator start) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Print (std::ostream &os) const;

  /**
  * \param extension Extension object.
  */
  void AddExtension (LtpExtension extension);
  /**
  * \return Extesion object
  * \param index Requested index
  */
  LtpExtension GetExtension (uint32_t index) const;

  bool operator == (LtpTrailer const &o) const;

private:
  /* Check m_extensions.size() is equal to m_trailerExtensionsCnt */
  std::vector<LtpExtension> m_extensions;       //!< Extensions in TLV format (from 0 to 15)

};


/**
 * \ingroup dtn
 *
 * \brief Generic Header for LTP data content as defined in RFC-5326 section 3.2
 */
class LtpContentHeader : public Header
{
public:
  class ReceptionClaim
  {
public:
    uint64_t offset;
    uint64_t length;

    bool operator < (ReceptionClaim const &o) const;
  };

  LtpContentHeader ();
  LtpContentHeader (SegmentType);
  ~LtpContentHeader ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Print (std::ostream &os) const;

  /* Setter Methods */

  /**
  * \param Segmentype enum value.
  */
  void SetSegmentType (SegmentType segmentType);
  /**
  * \param id Client service id.
  */
  void SetClientServiceId (uint64_t id);
  /**
  * \param val value for offset.
  */
  void SetOffset (uint64_t val);
  /**
  * \param val value for length parameter.
  */
  void SetLength (uint64_t val);
  /**
  * \param num value for checkpoint serial number
  */
  void SetCpSerialNumber (uint64_t num);
  /**
  * \param num value for report serial number
  */
  void SetRpSerialNumber (uint64_t num);
  /**
  * \param bound value for upper bound of report segment
  */
  void SetUpperBound (uint64_t bound);
  /**
  * \param bound value for lower bound of report segment
  */
  void SetLowerBound (uint64_t bound);
  /**
  * \param code Enum value for reason of cancellation
  */
  void SetCxReason (CxReasonCode code);
  /**
    * \brief Adds new reception claim and updates de reception claim counter.
    * \param claim Reception Claim object
    */
  void AddReceptionClaim (ReceptionClaim claim);


  /* Getter Methods */

  /**
  * \return Segmentype enum value.
  */
  SegmentType GetSegmentType () const;
  /**
  * \return Client service id.
  */
  uint64_t GetClientServiceId () const;
  /**
   * \return value for length parameter.
  */
  uint64_t GetOffset () const;
  /**
  * \return value for length parameter.
  */
  uint64_t GetLength () const;
  /**
   * \return value for checkpoint serial number
   */
  uint64_t GetCpSerialNumber () const;
  /**
   * \return value for report serial number
   */
  uint64_t GetRpSerialNumber () const;
  /**
  * \return value of upper bound of report segment
  */
  uint64_t GetUpperBound () const;
  /**
  * \return value of lower bound of report segment
  */
  uint64_t GetLowerBound () const;
  /**
  * \return Enum value for reason of cancellation
  */
  CxReasonCode GetCxReason () const;
  /**
  * \return number of contained reception claims
  */
  uint64_t GetRxClaimCnt () const;
  /**
   * \return ReceptionClaim object
   * \param Requested index
   */
  ReceptionClaim GetReceptionClaim (uint32_t index);
  /**
  * \brief Removes all reception claims.
  */
  void ClearReceptionClaims ();

  bool operator == (LtpContentHeader const &o) const;

private:
  SegmentType m_type;

  /* Data Segment */
  uint64_t m_clientServiceId; //!< Client Service Id (SDNV)
  uint64_t m_offset;              //!< Offset within session block (SDNV)
  uint64_t m_length;              //!< Length of data in octets (SDNV)

  /* Checkpoint or Report Segment*/
  uint64_t m_cpSerialNumber;  //!< Checkpoint identifier (SDNV)
  uint64_t m_rpSerialNumber;  //!< Report Serial Number (SDNV)

  /* Report Segment or ReportACK Segment*/

  uint64_t m_upperBound;        //!< Defines the higher bound of the scope for reception claims
  uint64_t m_lowerBound;        //!< Defines the lower bound of the scope for reception claims
  uint64_t m_rxClaimCnt;        //!< Number of reception claims in this segment

  std::vector<ReceptionClaim> m_rxClaims; //<! Reception Claims

  /* Cancel Segment */
  uint8_t m_cxReason;           //!< Reason of cancellation.
};

//} // namespace ltp
} // namespace ns3

#endif /* LTP_HEADER_H */

