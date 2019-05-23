﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
//---------------------------------------------------------
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

using System;
using System.Linq;

using Game.Protocol;
using Game.Class;
using FlatBuffers;

            //Debug.Log("이게 출력");

public class NetworkMessage
{
    public int LimitReceivebyte = 10000;                     // Receive Data Length. (byte)
    public byte[] Receivebyte = new byte[10000];    // Receive data by this array to save.
    public byte[] Sendbyte = new byte[10000];
    public int now_packet_size = 0;
    public int prev_packet_size = 0;
    public StringBuilder sb = new StringBuilder();

    public void set_prev(int value)
    {
        this.prev_packet_size = value;
    }
};

public class PacketData
{
    // 서버에서 패킷을 '패킷사이즈|패킷종류,데이터' 형식으로 보냄
    // 패킷을 한바이트씩 읽는데 |읽으면 멈추고 패킷사이즈 저장
    public int packet_size = 0;
    public int type_Pos = 0;
    public PacketData(int size, int pos)
    {
        this.packet_size = size;
        this.type_Pos = pos;
    }
};

namespace Game.Network
{
    public class NetWork : MonoBehaviour
    {
        public static Socket sock;
     
        private static byte[] Sendbyte = new byte[1024];

        public static Dictionary<int, ClientClass> client_data = new Dictionary<int, ClientClass>();

        public static Dictionary<int, MonsterClass> monster_data = new Dictionary<int, MonsterClass>();

        public static Dictionary<int, ItemClass> item_data = new Dictionary<int, ItemClass>();

        static SendFunc sF = new SendFunc();

        public static int Client_id = -1;         // 자신의 클라이언트 아이디

        float deltaTime = 0.0f;     // FPS 측정

        private static bool serverConnect = false;  // 서버 연결을 했는지 체크
        private static ManualResetEvent connectDone = new ManualResetEvent(false);
        private string playerIP = "";   // 플레이어 아이피
        private static NetWork instance_S = null;

        public static NetWork Instance_S
        {
            get
            {
                return instance_S;
            }
        }


        public static int new_player_id = 0;
        
        public string PlayerIP                 // 플레이어 IP 접근 프로퍼티
        {
            get
            {
                return playerIP;
            }
            set
            {
                playerIP = value;
            }
        }
        // Update is called once per frame
        void Update()
        {
            deltaTime += (Time.deltaTime - deltaTime) * 0.1f;
        }

        private void Awake()
        {
            // 변수 초기화, 처음 모두 0개
            instance_S = this;
            QualitySettings.vSyncCount = 0;
            Application.targetFrameRate = 60;
            Application.runInBackground = true;
            NetworkInit();
        }
        public void NetworkInit()
        {
            client_data.Clear();
        }

        public void ProcessPacket(int size, int type, byte[] recvPacket)
        {
            if (type == Game.Protocol.Protocol.SC_ID)
            {
                ByteBuffer recv_buf = new ByteBuffer(recvPacket);
                Client_info get_data = Client_info.GetRootAsClient_info(recv_buf);
                Client_id = get_data.Id;
                new_player_id = get_data.Id;
                int id = get_data.Id;
                int hp = get_data.Hp;
                int ani = get_data.Animator;
                float x = get_data.DirX;
                float z = get_data.DirZ;
                float h = get_data.Horizontal;
                float v = get_data.Vertical;
                string n = get_data.Name;
                Vector3 p = new Vector3(get_data.Position.Value.X, get_data.Position.Value.Y, get_data.Position.Value.Z);
                Vector3 r = new Vector3(get_data.Rotation.Value.X, get_data.Rotation.Value.Y, get_data.Rotation.Value.Z);

                client_data.Add(id, new ClientClass(id, hp, ani, x, z, h, v, n, p, r));
                client_data[id].set_draw(true);
                Debug.Log("클라이언트 아이디 : " + Client_id);
            }
            else if (type == Game.Protocol.Protocol.SC_PUT_PLAYER)      // 다른 클라가 접속했을 때
            {
                ByteBuffer recv_buf = new ByteBuffer(recvPacket);
                Client_info get_data = Client_info.GetRootAsClient_info(recv_buf);
                new_player_id = get_data.Id; 
                int id = get_data.Id;
                int hp = get_data.Hp;
                int ani = get_data.Animator;
                float x = get_data.DirX;
                float z = get_data.DirZ;
                float h = get_data.Horizontal;
                float v = get_data.Vertical;
                string n = get_data.Name;
                Vector3 p = new Vector3(get_data.Position.Value.X, get_data.Position.Value.Y, get_data.Position.Value.Z);
                Vector3 r = new Vector3(get_data.Rotation.Value.X, get_data.Rotation.Value.Y, get_data.Rotation.Value.Z);

                if (client_data.ContainsKey(id))
                {
                    ClientClass iter = client_data[id];
                    iter.set_hp(hp);
                    iter.set_pos(p);
                    iter.set_rot(r);
                    iter.set_vertical(v);
                    iter.set_horizontal(h);
                    iter.set_animator(ani);
                    iter.set_dirX(x);
                    iter.set_dirZ(z);
                    iter.set_draw(true);
                }
                else
                {
                    client_data.Add(id, new ClientClass(id, hp, ani, x, z, h, v, n, p, r));
                    client_data[id].set_draw(true);
                }
                Debug.Log("새로 접속한 아이디 : " + new_player_id);

            }
            else if (type == Game.Protocol.Protocol.SC_ALL_PLAYER_DATA)     // 이 클라가 처음 접속했을 때 다른 모든 클라들의 데이터
            {
                ByteBuffer recv_buf = new ByteBuffer(recvPacket);
                var get_all_data = Client_Collection.GetRootAsClient_Collection(recv_buf);
                for (int i = 0; i < get_all_data.DataLength; ++i) 
                {
                    //데이터 접근 get_all_data.Data(i).Value.변수
                    int id = get_all_data.Data(i).Value.Id;
                    int hp = get_all_data.Data(i).Value.Hp;
                    int ani = get_all_data.Data(i).Value.Animator;
                    float x = get_all_data.Data(i).Value.DirX;
                    float z = get_all_data.Data(i).Value.DirZ;
                    float h = get_all_data.Data(i).Value.Horizontal;
                    float v = get_all_data.Data(i).Value.Vertical;
                    string n = get_all_data.Data(i).Value.Name;
                    Vector3 p = new Vector3(get_all_data.Data(i).Value.Position.Value.X, get_all_data.Data(i).Value.Position.Value.Y, get_all_data.Data(i).Value.Position.Value.Z );
                    Vector3 r = new Vector3(get_all_data.Data(i).Value.Rotation.Value.X, get_all_data.Data(i).Value.Rotation.Value.Y, get_all_data.Data(i).Value.Rotation.Value.Z );

                    if(client_data.ContainsKey(id))
                    {
                        ClientClass iter = client_data[id];
                        iter.set_hp(hp);
                        iter.set_pos(p);
                        iter.set_rot(r);
                        iter.set_vertical(v);
                        iter.set_horizontal(h);
                        iter.set_animator(ani);
                        iter.set_dirX(x);
                        iter.set_dirZ(z);
                        iter.set_draw(true);
                    }
                    else
                    {
                        client_data.Add(id, new ClientClass(id, hp, ani, x, z, h, v, n, p, r));
                        client_data[id].set_draw(true);
                    }
                }
            }
            else if(type == Game.Protocol.Protocol.SC_PLAYER_STATUS)        //  클라가 움직였을때 그 클라 데이터
            {
                ByteBuffer recv_buf = new ByteBuffer(recvPacket);
                Client_info get_data = Client_info.GetRootAsClient_info(recv_buf);
                int id = get_data.Id;
                int hp = get_data.Hp;
                int ani = get_data.Animator;
                float x = get_data.DirX;
                float z = get_data.DirZ;
                float h = get_data.Horizontal;
                float v = get_data.Vertical;
                string n = get_data.Name;
                Vector3 p = new Vector3(get_data.Position.Value.X, get_data.Position.Value.Y, get_data.Position.Value.Z);
                Vector3 r = new Vector3(get_data.Rotation.Value.X, get_data.Rotation.Value.Y, get_data.Rotation.Value.Z);

                if (client_data.ContainsKey(id))
                {
                    ClientClass iter = client_data[id];
                    iter.set_hp(hp);
                    iter.set_pos(p);
                    iter.set_rot(r);
                    iter.set_vertical(v);
                    iter.set_horizontal(h);
                    iter.set_animator(ani);
                    iter.set_dirX(x);
                    iter.set_dirZ(z);
                    iter.set_draw(true);
                }
                else
                {
                        client_data.Add(id, new ClientClass(id, hp, ani, x, z, h, v, n, p, r));
                        client_data[id].set_draw(true);
                }
            }
            else if (type == Game.Protocol.Protocol.SC_REMOVE_PLAYER)
            {
                int id = recvPacket[0];
                client_data[id].set_draw(false);
            }
            else if(type == Game.Protocol.Protocol.SC_PUT_MONSTER)
            {
                ByteBuffer recv_buf = new ByteBuffer(recvPacket);
                Monster_info get_data = Monster_info.GetRootAsMonster_info(recv_buf);
                int id = get_data.Id;
                int hp = get_data.Hp;
                int ani = get_data.Animator;
                float x = get_data.DirX;
                float z = get_data.DirZ;
                Vector3 p = new Vector3(get_data.Position.Value.X, get_data.Position.Value.Y, get_data.Position.Value.Z);
                Vector3 r = new Vector3(get_data.Rotation.Value.X, get_data.Rotation.Value.Y, get_data.Rotation.Value.Z);

                if (monster_data.ContainsKey(id))
                {
                    MonsterClass iter = monster_data[id];
                    iter.set_hp(hp);
                    iter.set_pos(p);
                    iter.set_rot(r);
                    iter.set_animator(ani);
                    iter.set_dirX(x);
                    iter.set_dirZ(z);
                    iter.set_draw(true);
                }
                else
                {
                    monster_data.Add(id, new MonsterClass(id, hp, ani, x, z, p, r));
                    monster_data[id].set_draw(true);
                }
                Debug.Log("몬스터 생성 아이디 : " + id);

            }
            else if(type == Game.Protocol.Protocol.SC_PUT_ITEM)
            {
                ByteBuffer recv_buf = new ByteBuffer(recvPacket);
                var get_all_data = Item_Collection.GetRootAsItem_Collection(recv_buf);
                for (int i = 0; i < get_all_data.DataLength; ++i)
                {
                    //데이터 접근 get_all_data.Data(i).Value.변수
                    int id = get_all_data.Data(i).Value.Id;
                    int t = get_all_data.Data(i).Value.Type;
                    Vector3 p = new Vector3(get_all_data.Data(i).Value.Position.Value.X, get_all_data.Data(i).Value.Position.Value.Y, get_all_data.Data(i).Value.Position.Value.Z);

                    if (item_data.ContainsKey(id))
                    {
                        ItemClass iter = item_data[id];
                        iter.set_id(id);
                        iter.set_type(t);
                        iter.set_pos(p);
                        iter.set_draw(true);
                    }
                    else
                    {
                        item_data.Add(id, new ItemClass(id,t,p));
                        item_data[id].set_draw(true);
                    }
                }
            }
        }

        public static void Send_Packet(byte[] packet)
        {
            if (serverConnect == true)
            {
                try
                {
                    //Debug.Log(packet.Length);
                   sock.Send(packet, packet.Length, 0);
                }
                catch (SocketException err)
                {
                    Debug.Log("Socket send or receive error! : " + err.ToString());
                }
            }
        }

        public PacketData Get_packet_size(byte[] Receivebyte)
        {
           
            //-------------------------------------------------------------------------------------
            /*
             C++ itoa를 통한 char로 넣은것을 for문을 통하여 컨버팅 하여 가져온다.
             124는 C++에서 '|'값 이다.
             str_size로 실제 패킷 값을 계산해서 넣는다.
             */
            string str_size = "";
            string tmp_int = "";
            byte[] temp = new byte[8];
            int type_Pos = 0;

            for (type_Pos = 0; type_Pos < 8; ++type_Pos)
            {
                if (Receivebyte[type_Pos] == 124)
                    break;
                temp[0] = Receivebyte[type_Pos];
                tmp_int = Encoding.Default.GetString(temp);
                str_size += Int32.Parse(tmp_int);
            }
            //-------------------------------------------------------------------------------------

            return new PacketData(Int32.Parse(str_size), type_Pos);
        }

        public float DistanceToPoint(Vector3 a, Vector3 b)
        {
            // 캐릭터 간의 거리 구하기.
            return (float)Math.Sqrt(Math.Pow(a.x - b.x, 2) + Math.Pow(a.z - b.z, 2));
        }

        void ConnectCallback(IAsyncResult ar)
        {
            try
            {
                // 서버가 정상적으로 연결이 되었을 경우.

                connectDone.Set();
                if (sock.Connected == true)
                {
                    serverConnect = true;
                    Login_Button.login_ok = true;
                    Debug.Log("서버와 정상적으로 연결 되었습니다.");
                    RecieveHeader();//start recieve header
                }
                else
                {
                    Debug.Log("서버와 연결이 끊어졌습니다.");
                }
            }
            catch (Exception e)
            {
                connectDone.Set();
                Console.WriteLine(e.ToString());
            }
        }

        void RecieveHeader()
        {
            try
            {
                NetworkMessage msg = new NetworkMessage();
                sock.BeginReceive(msg.Receivebyte, 0, msg.LimitReceivebyte, SocketFlags.None, new AsyncCallback(RecieveHeaderCallback), msg);
            }
            catch (Exception e)
            {
                Debug.Log(e.Message);
            }
        }

        void RecieveHeaderCallback(IAsyncResult ar)
        {
            // 시작할때 데이터가 연달아 붙어서 와서 안되는것같다.
            // 받은 데이터를 계속 확인하면서 받은 크기만큼 끊고 이어서 다시 받고 하는 식으로 짜야할듯
            // 데이터가 덜 왔다면 기다렸다가 크기만큼 받고
            // 데이터가 뒤에꺼까지 같이 왔다면 앞부분만큼 잘라 읽기
            try
            {
                NetworkMessage msg = (NetworkMessage)ar.AsyncState;     // Recieve된 Packet을 받아온다.
                int bytesRead = sock.EndReceive(ar);        // 소켓에서 받아온 사이즈를 확인한다.

                PacketData size_data = Get_packet_size(msg.Receivebyte);

                int recv_packet_size = size_data.packet_size;
                int packet_type = msg.Receivebyte[size_data.type_Pos + 1]; // 패킷 타입
                //size_data. type_Pos 가 앞에 데이터 크기|패킷 타입

                if (recv_packet_size == bytesRead)
                {
                    // 소켓에서 받은 데이터와 실제 패킷 사이즈가 같을 경우
                    byte[] recv_byte = new byte[recv_packet_size];

                    for (int i = 8; i < recv_packet_size + 8; ++i)
                    {
                        recv_byte[i - 8] = msg.Receivebyte[i];
                    }
                    ProcessPacket(recv_packet_size, packet_type, recv_byte);

                    // 패킷 처리가 완료 되었으니 다시 리시브 상태로 돌아간다.
                    NetworkMessage new_msg = new NetworkMessage();
                    sock.BeginReceive(new_msg.Receivebyte, 0, new_msg.LimitReceivebyte, SocketFlags.None, new AsyncCallback(RecieveHeaderCallback), new_msg);
                }
                else
                {
                    // 소켓에서 받은 데이터와 실제 패킷 사이즈가 다를 경우
                    msg.sb.Append(Encoding.ASCII.GetString(msg.Receivebyte, 0, bytesRead));
                    msg.set_prev(bytesRead);
                    // 소켓에서 받은 데이터가 안맞는 경우 패킷이 뒤에 붙어서 오는거 같은 느낌이 든다...
                    size_data = Get_packet_size(msg.Receivebyte);
                    recv_packet_size = size_data.packet_size;
                    packet_type = msg.Receivebyte[size_data.type_Pos + 1]; // 패킷 타입
                    byte[] recv_byte = new byte[recv_packet_size];

                    for (int i = 8; i < recv_packet_size + 8; ++i)
                    {
                        recv_byte[i - 8] = msg.Receivebyte[i];
                    }

                    ProcessPacket(recv_packet_size, packet_type, recv_byte);

                    sock.BeginReceive(msg.Receivebyte, 0, msg.LimitReceivebyte, SocketFlags.None, new AsyncCallback(RecieveHeaderCallback), msg);
                }
            }
            catch
            {
                NetworkMessage new_msg = new NetworkMessage();
                sock.BeginReceive(new_msg.Receivebyte, 0, new_msg.LimitReceivebyte, SocketFlags.None, new AsyncCallback(RecieveHeaderCallback), new_msg);
            }
        }

        public void connectSocket()
        {
            if (serverConnect == false)
            {     // Mode가 0이 될 경우 = 플레이어가 로그인 한 이후
                sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                sock.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, 1);
                sock.NoDelay = true;
                if (playerIP == "")
                    sock.BeginConnect("127.0.0.1", 9000, new AsyncCallback(ConnectCallback), sock);
                else
                    sock.BeginConnect(playerIP, 9000, new AsyncCallback(ConnectCallback), sock);
            }
        }


        void OnApplicationQuit()
        {
            sock.Close();
            sock = null;
        }

        public static void SendPlayerInfo(Vector3 p, int ani, float x, float z, float h, float v, Vector3 rot, string nick)
        {
            Sendbyte = sF.makeClient_PacketInfo(p, ani, x, z, h, v, rot, nick);
            Send_Packet(Sendbyte);
        }
    }
}