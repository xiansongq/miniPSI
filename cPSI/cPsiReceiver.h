#pragma  once

#include "volePSI/Defines.h"
#include "volePSI/config.h"
#include "sodium.h"

#include "cryptoTools/Common/BitVector.h"
#include "cryptoTools/Common/CuckooIndex.h"
#include "cryptoTools/Common/Timer.h"
#include "cryptoTools/Crypto/PRNG.h"
#include "cryptoTools/Network/Channel.h"
#include "volePSI/GMW/Gmw.h"
#include "volePSI/RsOpprf.h"
#include "volePSI/SimpleIndex.h"

namespace volePSI {
class cPsiReceiver : public oc::TimerAdapter {
 public:
  enum ValueShareType { Xor, add32 };
  u64 senderSize, receiverSize;
  u64 mValueByteLength = 0;
  u64 mSsp = 0;
  u64 numThreads = 0;
  PRNG mPrng;
  ValueShareType mType = ValueShareType::Xor;
  struct Sharing {
    // The sender's share of the bit vector indicating that
    // the i'th row is a real row (1) or a row (0).
    oc::BitVector mFlagBits;

    // Secret share of the values associated with the output
    // elements. These values are from the sender.
    oc::Matrix<u8> mValues;

    // The mapping of the senders input rows to output rows.
    // Each input row might have been mapped to one of three
    // possible output rows.
    std::vector<std::array<u64, 3>> mMapping;
  };
  void init(u64 senderSize, u64 receiverSize, u64 mValueByteLength, u64 mSSp,
            u64 numThreads, block seed, ValueShareType mType);
  void receive(span<block> X, Sharing& ret, Socket& chl);
};

}  // namespace volePSI
