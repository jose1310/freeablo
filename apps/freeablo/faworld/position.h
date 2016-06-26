#ifndef POSITION_H
#define POSITION_H

#include <utility>
#include <stdlib.h>
#include <stdint.h>
#include <cmath>
#include "../fasavegame/savegame.h"
#include "netobject.h"

#include <misc/misc.h>
#include <serial/bitstream.h>

namespace FAWorld
{
    class Position : public NetObject
    {
        public:
            Position();
            Position(int32_t x, int32_t y);
            Position(int32_t x, int32_t y, int32_t direction);

            void update(); ///< advances towards mNext
            std::pair<int32_t, int32_t> current() const; ///< where we are coming from
            std::pair<int32_t, int32_t> next() const; ///< where we are going to
            int32_t mDist; ///< percentage of the way there
            int32_t mDirection;
            bool mMoving;
            double distanceFrom(Position B);
            virtual size_t getWriteSize();
            virtual bool writeTo(ENetPacket *packet, size_t& position);
            virtual bool readFrom(ENetPacket *packet, size_t& position);

        
        private:
            std::pair<int32_t, int32_t> mCurrent;

            template<class Archive>
            void save(Archive & ar, const unsigned int version) const
            {
                UNUSED_PARAM(version);

                ar << mDirection;
                ar << mCurrent;
            }

            template<class Archive>
            void load(Archive & ar, const unsigned int version)
            {
                UNUSED_PARAM(version);

                ar >> mDirection;
                ar >> mCurrent;
                mMoving = false;
                mDist = 0;
            }


            friend class boost::serialization::access;

            BOOST_SERIALIZATION_SPLIT_MEMBER()

            template<class Stream> 
            bool faSerial(Stream& stream)
            {
                int64_t pos = stream.tell();

                bool success = true;
                success &= stream.handleInt<0, 100>(mDist);
                success &= stream.handleInt<0, 7>(mDirection);
                success &= stream.handleBool(mMoving);
                success &= stream.handleInt32(mCurrent.first);
                success &= stream.handleInt32(mCurrent.second);

                if (!success)
                    stream.seek(pos, Serial::BSPos::Start);

                return success;
            }
    };
}

#endif
