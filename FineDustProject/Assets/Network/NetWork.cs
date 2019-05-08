using System.Collections;
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
    public int LimitReceivebyte = 1024;                     // Receive Data Length. (byte)
    public byte[] Receivebyte = new byte[1024];    // Receive data by this array to save.
    public byte[] Sendbyte = new byte[1024];
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

        Vector3 Player_Position;
        Vector3 Player_Rotation;

        private byte[] Sendbyte = new byte[1024];

        public static Dictionary<int, ClientClass> client_data = new Dictionary<int, ClientClass>();

        Game.Protocol.Protocol recv_protocol = new Game.Protocol.Protocol();
        SendFunc sF = new SendFunc();

        public static int Client_id = -1;         // 자신의 클라이언트 아이디
        public int get_id() { return Client_id; }

        float deltaTime = 0.0f;     // FPS 측정

        private static bool serverConnect = false;  // 서버 연결을 했는지 체크
        private static ManualResetEvent connectDone = new ManualResetEvent(false);
        private string playerIP = "";   // 플레이어 아이피
        private static NetWork instance_S= null; // 정적 변수

        public static NetWork Instance_S  // 인스턴스 접근 프로퍼티
        {
            get
            {
                return instance_S;
            }
        }

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
            //item_Collection.Clear();
        }
        //IEnumerator startPrefab()
        //{
        //    do
        //    {
        //        // 플레이어 관련한 프리팹
        //        foreach (var key in client_data.Keys.ToList())
        //        {
        //            if (client_data[key].get_id() == Client_id)// 나면
        //            {
        //                Player_Script.ID = Client_id;
        //                Player_Script.hp = client_data[key].get_hp();
        //               if (client_data[key].get_draw() == false)
        //                {
        //                    //죽었을때 (안그릴때)
        //                    Player_Script.hp = 0;
        //                    Player_Script.Die = true;
        //                }

        //                continue;
        //            }
        //            if (client_data[key].get_connect() == true && client_data[key].get_prefab() == false)
        //            {// 내가 아닌 다른 플레이어가 연결되어 있는데 프리팹이 생성 안된 경우
        //                client_data[key].player = Instantiate(PrefabPlayer, client_data[key].get_pos(), Quaternion.identity);
        //                client_data[key].player.transform.SetParent(OtherPlayerCollection.transform);

        //                client_data[key].script = client_data[key].player.GetComponent<OtherPlayerCtrl>();
        //                client_data[key].script.otherPlayer_id = client_data[key].get_id();

        //                // 처음 위치를 넣어 줘야 한다. 그러지 않을경우 다른 클라이언트 에서는 0,0 에서부터 천천히 올라오게 보인다
        //                client_data[key].player.transform.position = client_data[key].get_pos();
        //            }
        //            else if (client_data[key].get_prefab() == true)
        //            {
        //                // 플레이어 프리팹이 정상적으로 생성 되었을 경우.
        //                if (client_data[key].player.activeSelf == false && client_data[key].get_activePlayer() == true)
        //                {
        //                    // setActive가 꺼져 있을 경우, 코루틴이랑 같이 활성화 시킨다.
        //                    client_data[key].player.SetActive(true);
        //                    client_data[key].script.StartCoroutine(client_data[key].script.createPrefab());
        //                    client_data[key].set_activePlayer(false);
        //                }

        //                if (client_data[key].get_hp() <= 0 && client_data[key].get_draw() == true)
        //                {
        //                    // 서버에서 해당 클라이언트 체력이 0 이고 죽는게 허가 된 경우
        //                    client_data[key].set_hp(0);
        //                    client_data[key].script.OtherPlayerDie();
        //                }
                        
        //                else
        //                {
        //                    // 실제로 캐릭터를 움직이는 것은 코루틴 여기서 움직임을 진행 한다.
        //                    client_data[key].script.set_hp(client_data[key].get_hp());
        //                    client_data[key].script.MovePos(client_data[key].get_pos());
        //                    client_data[key].script.collider_script.enabled = true;
        //                    Vector3 rotation = client_data[key].get_rot();
        //                    client_data[key].player.transform.rotation = Quaternion.Euler(rotation.x, rotation.y, rotation.z);
        //                }
        //            }
        //            if (client_data[key].get_draw() == false)
        //            {
        //                // 플레이어 삭제를 할 경우 SetActive를 꺼준다.
        //                client_data[key].set_activePlayer(false);
        //                client_data[key].player.SetActive(false);
        //            }
        //        }
        //        yield return null;
        //    } while (true);
        //}

        public void ProcessPacket(int size, int type, byte[] recvPacket)
        {
            if (type == Game.Protocol.Protocol.SC_ID)
            {
                Client_id = recvPacket[2];
                
                //----------------------------------------------------------------
                // 클라이언트 아이디가 정상적으로 받은건지 확인을 한다. recvPacket = 타입 | 아이디
                //----------------------------------------------------------------
                Debug.Log("클라이언트 아이디 : " + Client_id);
            }
            else if (type == Game.Protocol.Protocol.SC_PUT_PLAYER)
            {
                // 클라이언트 하나에 대한 데이터가 들어온다.
                byte[] temp_buf = new byte[size + 1];
                System.Buffer.BlockCopy(recvPacket, 8, temp_buf, 0, size); // 사이즈를 제외한 실제 패킷값을 복사한다.
                ByteBuffer revc_buf = new ByteBuffer(temp_buf); // ByteBuffer로 byte[]로 복사한다.

                var Get_ServerData = Client_info.GetRootAsClient_info(revc_buf);

                // 클라이언트 데이터에 서버에서 받은 데이터를 넣어준다.
                if (client_data.ContainsKey(Get_ServerData.Id))
                {
                    // 이미 값이 들어가 있는 상태라면
                    ClientClass iter = client_data[Get_ServerData.Id];
                    iter.set_pos(new Vector3(Get_ServerData.Position.Value.X, Get_ServerData.Position.Value.Y, Get_ServerData.Position.Value.Z));
                    iter.set_rot(new Vector3(Get_ServerData.Rotation.Value.X, Get_ServerData.Rotation.Value.Y, Get_ServerData.Rotation.Value.Z));
                }
                else
                {
                    // 클라이언트가 자기 자신이 아닐경우에만 추가해준다.
                    client_data.Add(Get_ServerData.Id, new ClientClass(Get_ServerData.Id, Get_ServerData.Hp, Get_ServerData.Name.ToString(), new Vector3(Get_ServerData.Position.Value.X, Get_ServerData.Position.Value.Y, Get_ServerData.Position.Value.Z), new Vector3(Get_ServerData.Rotation.Value.X, Get_ServerData.Rotation.Value.Y, Get_ServerData.Rotation.Value.Z)));
                }
            }
            //else if (type == recv_protocol.SC_ALL_PLAYER_DATA)
            //{
            //    // 클라이언트 모든 데이터가 들어온다.
            //    byte[] temp_buf = new byte[size + 1];
            //    System.Buffer.BlockCopy(recvPacket, 8, temp_buf, 0, size); // 사이즈를 제외한 실제 패킷값을 복사한다.
            //    ByteBuffer revc_buf = new ByteBuffer(temp_buf); // ByteBuffer로 byte[]로 복사한다.
            //    var Get_ServerData = Client_Collection.GetRootAsClient_Collection(revc_buf);

            //    // 서버에서 받은 데이터 묶음을 확인하여 묶음 수 만큼 추가해준다.
            //    for (int i = 0; i < Get_ServerData.DataLength; i++)
            //    {
            //        if (client_data.ContainsKey(Get_ServerData.Data(i).Value.Id))
            //        {
            //            // 이미 값이 들어가 있는 상태라면
            //            ClientClass iter = client_data[Get_ServerData.Data(i).Value.Id];
            //            if (Get_ServerData.Data(i).Value.Hp > 0)
            //            {
            //                // 플레이어가 죽지 않은 경우에만 위치, 회전 값을 받아온다.
            //                iter.set_pos(new Vector3(Get_ServerData.Data(i).Value.Position.Value.X, Get_ServerData.Data(i).Value.Position.Value.Y, Get_ServerData.Data(i).Value.Position.Value.Z));
            //                iter.set_rot(new Vector3(Get_ServerData.Data(i).Value.Rotation.Value.X, Get_ServerData.Data(i).Value.Rotation.Value.Y, Get_ServerData.Data(i).Value.Rotation.Value.Z));
            //            }
            //            iter.set_hp(Get_ServerData.Data(i).Value.Hp);
            //            iter.set_horizontal(Get_ServerData.Data(i).Value.Horizontal);
            //            iter.set_vertical(Get_ServerData.Data(i).Value.Vertical);
            //            //if (iter.get_prefab() == true)
            //            //{
            //            //    // 프리팹이 만들어진 이후 부터 script를 사용할 수 있기 때문에 그 이후 애니메이션 동기화를 시작한다.
            //            //    iter.script.get_Animator(Get_ServerData.Data(i).Value.Animator);
            //            //    iter.script.Vertical = Get_ServerData.Data(i).Value.Vertical;
            //            //    iter.script.Horizontal = Get_ServerData.Data(i).Value.Horizontal;
            //            //}
            //        }
            //        else
            //        {
            //            // 클라이언트가 자기 자신이 아닐경우에만 추가해준다.
            //            client_data.Add(Get_ServerData.Data(i).Value.Id, new ClientClass(Get_ServerData.Data(i).Value.Id, Get_ServerData.Data(i).Value.Hp, Get_ServerData.Data(i).Value.Name.ToString(), new Vector3(Get_ServerData.Data(i).Value.Position.Value.X, Get_ServerData.Data(i).Value.Position.Value.Y, Get_ServerData.Data(i).Value.Position.Value.Z), new Vector3(Get_ServerData.Data(i).Value.Rotation.Value.X, Get_ServerData.Data(i).Value.Rotation.Value.Y, Get_ServerData.Data(i).Value.Rotation.Value.Z)));
            //        }
            //    }
            //}
            //else if (type == recv_protocol.SC_REMOVE_PLAYER)
            //{
            //    // 서버에서 내보낸 클라이언트를 가져 온다.
            //    byte[] temp_buf = new byte[size + 1];
            //    System.Buffer.BlockCopy(recvPacket, 8, temp_buf, 0, size); // 사이즈를 제외한 실제 패킷값을 복사한다.
            //    ByteBuffer revc_buf = new ByteBuffer(temp_buf); // ByteBuffer로 byte[]로 복사한다.
            //    var Get_ServerData = Client_Id_Packet.GetRootAsClient_Id_Packet(revc_buf);

            //    ClientClass iter = client_data[Get_ServerData.Id];
            //    // 해당 클라이언트의 SetActive를 꺼준다.
            //    iter.set_draw(false);
            //}
        }

        public void Send_Packet(byte[] packet)
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
                    ProcessPacket(recv_packet_size, packet_type, msg.Receivebyte);

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
                    byte[] recv_byte = new byte[size_data.packet_size + 9];

                    for (int i = 0; i < size_data.packet_size; ++i)
                    {
                        recv_byte[i] = msg.Receivebyte[i];
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
    }
}