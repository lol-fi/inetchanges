//
// Copyright (C) 2005 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "inet/common/INETDefs.h"

#include "inet/common/queue/ReturnTailQueue.h"

namespace inet {

Define_Module(ReturnTailQueue);

simsignal_t ReturnTailQueue::queueLengthSignal = registerSignal("queueLength");

void ReturnTailQueue::initialize()
{
    PassiveQueueBase::initialize();

    queue.setName(par("queueName"));

    //statistics
    emit(queueLengthSignal, queue.getLength());

    outGate = gate("out");

    // configuration
    frameCapacity = par("frameCapacity");
    threshold = par("threshold");

}

cMessage *ReturnTailQueue::enqueue(cMessage *msg)
{
    EV << "ReturnTailQueue enqueue " << msg << endl;
    //return beyond threshold
    if (frameCapacity && queue.getLength() >= threshold) {
        EV << "ReturnTailQueue full, returning packet.\n";
        return nullptr;
    }
    else {
        queue.insert(msg);
        emit(queueLengthSignal, queue.getLength());
        return nullptr;
    }
}

cMessage *ReturnTailQueue::dequeue()
{
    if (queue.isEmpty())
        return nullptr;

    cMessage *msg = static_cast<cMessage *>(queue.pop());

    // statistics
    emit(queueLengthSignal, queue.getLength());

    return msg;
}

void ReturnTailQueue::sendOut(cMessage *msg)
{
    send(msg, outGate);
}

bool ReturnTailQueue::tooFull()
{
    return (queue.getLength() >= threshold);
}

bool ReturnTailQueue::isEmpty()
{
    return queue.isEmpty();
}

} // namespace inet

