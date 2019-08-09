using System.Collections;
using System.Collections.Generic;
using System;
using System.Text;

using UnityEngine;
using FlatBuffers;
using Game.Protocol;
using Game.Class;

namespace Game.Network
{
    public class SendFunc: Game.Protocol.Protocol
    {
        public byte[] makeClient_PacketInfo(Vector3 pos, int Player_Animator,float dirX, float dirZ, float horizontal, float vertical, Vector3 PlayerRotation, string nickName)
        {
            FlatBufferBuilder fbb = new FlatBufferBuilder(1);
            fbb.Clear(); // 클리어를 안해주고 시작하면 계속 누적해서 데이터가 들어간다.
            var fbbNickName = fbb.CreateString(nickName); // String 문자열이 있을경우 미리 생성해라.
            Client_info.StartClient_info(fbb);
            Client_info.AddAnimator(fbb, Player_Animator);
            Client_info.AddDirX(fbb, dirX);
            Client_info.AddDirZ(fbb, dirZ);
            Client_info.AddHorizontal(fbb, horizontal);
            Client_info.AddVertical(fbb, vertical);
            Client_info.AddName(fbb, fbbNickName);
            Client_info.AddPosition(fbb, Vec3.CreateVec3(fbb, pos.x, pos.y, pos.z));
            Client_info.AddRotation(fbb, Vec3.CreateVec3(fbb, PlayerRotation.x, PlayerRotation.y, PlayerRotation.z));
            var endOffset = Client_info.EndClient_info(fbb);
            fbb.Finish(endOffset.Value);

            byte[] packet = fbb.SizedByteArray();   // flatbuffers 실제 패킷 데이터
            byte[] magic_packet = makePacketinfo(packet.Length, CS_INFO);
            byte[] real_packet = new byte[packet.Length + 8];
            System.Buffer.BlockCopy(magic_packet, 0, real_packet, 0, magic_packet.Length);
            System.Buffer.BlockCopy(packet, 0, real_packet, 8, packet.Length);

            return real_packet;
        }

        public byte[] makeEatItemPacket(int item, int player)
        {
            FlatBufferBuilder fbb = new FlatBufferBuilder(1);
            fbb.Clear();
            Eat_Item.StartEat_Item(fbb);
            Eat_Item.AddItemID(fbb, item);
            Eat_Item.AddPlayerID(fbb, player);
            var endOffset = Eat_Item.EndEat_Item(fbb);
            fbb.Finish(endOffset.Value);

            byte[] packet = fbb.SizedByteArray();   // flatbuffers 실제 패킷 데이터
            byte[] magic_packet = makePacketinfo(packet.Length, CS_GET_ITEM);
            byte[] real_packet = new byte[packet.Length + 8];
            System.Buffer.BlockCopy(magic_packet, 0, real_packet, 0, magic_packet.Length);
            System.Buffer.BlockCopy(packet, 0, real_packet, 8, packet.Length);

            return real_packet;

        }

        public byte[] makeLoginInfoPacket(string id, string pw)
        {
            FlatBufferBuilder fbb = new FlatBufferBuilder(1);
            fbb.Clear();
            var idOffset = fbb.CreateString(id); // String 문자열이 있을경우 미리 생성해라.
            var pwOffset = fbb.CreateString(pw); // String 문자열이 있을경우 미리 생성해라.
            Login.StartLogin(fbb);
            Login.AddId(fbb, idOffset);
            Login.AddPassword(fbb, pwOffset);
            var endOffset = Login.EndLogin(fbb);
            fbb.Finish(endOffset.Value);

            byte[] packet = fbb.SizedByteArray();
            byte[] magic_packet = makePacketinfo(packet.Length, CS_LOGIN);
            byte[] real_packet = new byte[packet.Length + 8];
            System.Buffer.BlockCopy(magic_packet, 0, real_packet, 0, magic_packet.Length);
            System.Buffer.BlockCopy(packet, 0, real_packet, 8, packet.Length);

            return real_packet;
        }

        public byte[] makeCharacterSelectPacket(int num)
        {
            byte[] packet = BitConverter.GetBytes(num);
            if (BitConverter.IsLittleEndian)
                Array.Reverse(packet);
            byte[] result = packet;
            byte[] magic_packet = makePacketinfo(packet.Length, CS_CHARACTER_SELECT);
            byte[] real_packet = new byte[packet.Length + 8];
            System.Buffer.BlockCopy(magic_packet, 0, real_packet, 0, magic_packet.Length);
            System.Buffer.BlockCopy(packet, 0, real_packet, 8, packet.Length);

            return real_packet;
        }

        public byte[] makePacketinfo(int p_size, int type)
        {
            byte[] intBytes = new byte[p_size.ToString().Length + 2];   // 숫자 길이 + | + type
            byte[] cpy_size = Encoding.UTF8.GetBytes(p_size.ToString());
            byte[] cpy_type = Encoding.UTF8.GetBytes(type.ToString());

            for (int i = 0; i < p_size.ToString().Length; ++i)
            {
                intBytes[i] = cpy_size[i];
                //intBytes[i] -= 48;
            }
            intBytes[p_size.ToString().Length] = 124;
            if (type >= 10)
            {
                // 10보다 큰 경우 앞 숫자가 아닌 뒷 숫자로 판별한다.
                intBytes[p_size.ToString().Length + 1] = cpy_type[1];
                intBytes[p_size.ToString().Length + 1] -= 38;
            }
            else
            {
                intBytes[p_size.ToString().Length + 1] = cpy_type[0];
                intBytes[p_size.ToString().Length + 1] -= 48;
            }

            // 48을 마이너스 해준 이유는 Server에서 packet[i]로 형 변환 없이 바로 값을 확인하기 위하여

            return intBytes;
        }
    }
}