//[Mavlink
import { MavLinkProtocolV1, send} from 'node-mavlink'
import { MavLinkPacketSplitter, MavLinkPacketParser } from 'node-mavlink'

import {
  minimal, common, ardupilotmega
} from 'node-mavlink'

import { SerialPort } from 'serialport'


const REGISTRY = {
  ...minimal.REGISTRY,
  ...common.REGISTRY,
  ...ardupilotmega.REGISTRY,
}


const msg2= new minimal.Heartbeat

const port = new SerialPort({ path: '/dev/ttyUSB0', baudRate: 115200 })

port.on('open', async () => {
  // the port is open - we're ready to send data
  while(true){
    await new Promise(resolve => setTimeout(resolve,10));
    await send(port, msg, new MavLinkProtocolV1())
  }
})

const reader = port
  .pipe(new MavLinkPacketSplitter())
  .pipe(new MavLinkPacketParser())

reader.on('data', packet => {
  const clazz = REGISTRY[packet.header.msgid]
  if (clazz) {
    const data = packet.protocol.data(packet.payload, clazz)
    console.log('Received packet:', data)
  }
})

//]