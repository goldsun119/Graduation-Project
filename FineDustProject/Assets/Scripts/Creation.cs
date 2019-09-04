using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Creation : MonoBehaviour
{
    private Inventory Iv;

    private Slot it0;
    private Slot it1;
    private Slot it2;
    private Slot it3;

    public int select_list_num;

    private Slot[] item_slot;

    public Button bb1;
    public Image img1;
    private bool b1;

    public Button bb2;
    public Image img2;
    private bool b2;

    public Button bb3;
    public Image img3;
    private bool b3;

    public Button bb4;
    public Image img4;
    private bool b4;

    public Button bb5;
    public Image img5;
    private bool b5;

    public Image item1;
    public Image item2;
    public Image item3;
    public Image item4;

    public Sprite item_img1;
    public Sprite item_img2;
    public Sprite item_img3;
    public Sprite item_clear;

    public Button ending;

    public Text name_text;
    public Text detail_text1;
    public Text detail_text2;
    public Text detail_text3;
    public Text detail_text4;

    // Start is called before the first frame update
    void Start()
    {
        Iv = GameObject.FindGameObjectWithTag("Inventory").GetComponent<Inventory>();

        //for (int i = 0; i < Iv.AllSlot.Count; i++)
        //{
        //    it[1i] = Iv.gameObject.transform.GetChild(i).GetComponent<Slot>();
        //}

        it0 = Iv.gameObject.transform.GetChild(0).GetComponent<Slot>();
        it1 = Iv.gameObject.transform.GetChild(1).GetComponent<Slot>();

        //it2 = Iv.gameObject.transform.GetChild(2).GetComponent<Slot>();

        //for (int i = 0; i < Iv.AllSlot.Count; i++)
        //{
        //    if (Iv.AllSlot[i].GetComponent<Slot>().ItemReturn().type == Item.TYPE.Box)
        //        item_slot[0] = Iv.AllSlot[i].GetComponent<Slot>();
        //    else if (Iv.AllSlot[i].GetComponent<Slot>().ItemReturn().type == Item.TYPE.Crystal)
        //        item_slot[1] = Iv.AllSlot[i].GetComponent<Slot>();
        //}

        select_list_num = 0;

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.M))
            Debug.Log("슬롯 개수" + Iv.AllSlot.Count);



        if (Game.Network.NetWork.product_complete[0] == 1)
        {
            img1.gameObject.SetActive(true);
            bb1.gameObject.SetActive(false);
            b1 = true;
        }
        if (Game.Network.NetWork.product_complete[1] == 1)
        {
            img2.gameObject.SetActive(true);
            bb2.gameObject.SetActive(false);
            b2 = true;
        }
        if (Game.Network.NetWork.product_complete[2] == 1)
        {
            img3.gameObject.SetActive(true);
            bb3.gameObject.SetActive(false);
            b3 = true;
        }
        if (Game.Network.NetWork.product_complete[3] == 1)
        {
            img4.gameObject.SetActive(true);
            bb4.gameObject.SetActive(false);
            b4 = true;
        }
        if (Game.Network.NetWork.product_complete[4] == 1)
        {
            img5.gameObject.SetActive(true);
            bb5.gameObject.SetActive(false);
            b5 = true;
        }
        
        if (b5)
        {
            ending.gameObject.SetActive(true);
        }

    }

    public void List1Button()
    {
        select_list_num = 1;
        name_text.text = "구름씨 제작기";
        item1.gameObject.SetActive(true);
        item2.gameObject.SetActive(false);
        item3.gameObject.SetActive(false);
        item4.gameObject.SetActive(false);

        detail_text1.gameObject.SetActive(true);
        detail_text2.gameObject.SetActive(false);
        detail_text3.gameObject.SetActive(false);
        detail_text4.gameObject.SetActive(false);
        if (it0.ItemReturn().type == Item.TYPE.Box)
            detail_text1.text = "수수께끼상자" + it0.slot.Count.ToString() + "/1";
   
        else
            detail_text1.text = "수수께끼상자" + it1.slot.Count.ToString() + "/1";

    }

    public void List2Button()
    {
        select_list_num = 2;
        name_text.text = "구름씨 도포기";
        item1.gameObject.SetActive(true);
        item2.gameObject.SetActive(false);
        item3.gameObject.SetActive(false);
        item4.gameObject.SetActive(false);

        detail_text1.gameObject.SetActive(true);
        detail_text2.gameObject.SetActive(false);
        detail_text3.gameObject.SetActive(false);
        detail_text4.gameObject.SetActive(false);

        if (it1.ItemReturn().type == Item.TYPE.Crystal)
            detail_text1.text = "삼각플라스크" + it1.slot.Count.ToString() + "/1";

        else
            detail_text1.text = "삼각플라스크" + it0.slot.Count.ToString() + "/1";
    }

    public void List3Button()
    {
        select_list_num = 3;
        name_text.text = "구름씨 배달기";
        item1.gameObject.SetActive(true);
        item2.gameObject.SetActive(true);
        item3.gameObject.SetActive(false);
        item4.gameObject.SetActive(false);

        detail_text1.gameObject.SetActive(true);
        detail_text2.gameObject.SetActive(true);
        detail_text3.gameObject.SetActive(false);
        detail_text4.gameObject.SetActive(false);

       if (it1.ItemReturn().type == Item.TYPE.Box)
            detail_text1.text = "수수께끼상자" + it1.slot.Count.ToString() + "/1";
        else
            detail_text1.text = "수수께끼상자" + it0.slot.Count.ToString() + "/1";

        if (it1.ItemReturn().type == Item.TYPE.Crystal)
            detail_text2.text = "삼각플라스크" + it1.slot.Count.ToString() + "/1";
        else
            detail_text2.text = "삼각플라스크" + it0.slot.Count.ToString() + "/1";
    }

    public void List4Button()
    {
        select_list_num = 4;
        name_text.text = "강우 촉진기";
        item1.gameObject.SetActive(true);
        item2.gameObject.SetActive(true);
        item3.gameObject.SetActive(false);
        item4.gameObject.SetActive(false);

        detail_text1.gameObject.SetActive(true);
        detail_text2.gameObject.SetActive(true);
        detail_text3.gameObject.SetActive(false);
        detail_text4.gameObject.SetActive(false);

        if (it1.ItemReturn().type == Item.TYPE.Box)
            detail_text1.text = "수수께끼상자" + it1.slot.Count.ToString() + "/1";
        else
            detail_text1.text = "수수께끼상자" + it0.slot.Count.ToString() + "/1";

        if (it1.ItemReturn().type == Item.TYPE.Crystal)
            detail_text2.text = "삼각플라스크" + it1.slot.Count.ToString() + "/1";
        else
            detail_text2.text = "삼각플라스크" + it0.slot.Count.ToString() + "/1";
    }

    public void List5Button()
    {
        select_list_num = 5;
        name_text.text = "인공구름 생성기";
        item1.gameObject.SetActive(true);
        item2.gameObject.SetActive(true);
        item3.gameObject.SetActive(true);
        item4.gameObject.SetActive(true);

        detail_text1.gameObject.SetActive(true);
        detail_text2.gameObject.SetActive(true);
        detail_text3.gameObject.SetActive(true);
        detail_text4.gameObject.SetActive(true);
        // 1,2,3,4 아이템 다 있어야 해서 4칸까지 있어야 함.
        //text1 = transform.Find("Button").gameObject.transform.Find("Text").gameObject.GetComponent<Text>();

        if (b1)
            detail_text1.text = "구름씨 제작기" + "1/1";
        else
            detail_text1.text = "구름씨 제작기" + "0/1";
        if (b2)
            detail_text2.text = "구름씨 도포기" + "1/1";
        else
            detail_text2.text = "구름씨 도포기" + "0/1";
        if (b3)
            detail_text3.text = "구름씨 배달기" + "1/1";
        else
            detail_text3.text = "구름씨 배달기" + "0/1";
        if (b4)
            detail_text4.text = "강우 촉진기" + "1/1";
        else
            detail_text4.text = "강우 촉진기" + "0/1";

    }


    public void List1Create()
    {

        switch (select_list_num)
        {
            case 1:
                // 걍 노가다로 때려박기
                //if (select_list_num == 1)
                {
                    //img.gameObject.SetActive(true);
                    //bb.gameObject.SetActive(false);
                    //b1 = true;
                    // 요구하는 아이템 리스트 if문으로 다 걸으세요.
                    if (it0.ItemReturn().type == Item.TYPE.Box && it0.slot.Count >= 1)
                    {
                        //img.gameObject.SetActive(true);
                        it0.ItemUse();
                        detail_text1.text = "수수께끼상자" + it0.slot.Count.ToString() + "/1";

                        //bb.gameObject.SetActive(false);
                        //b1 = true;

                        Game.Network.NetWork.SendCompleteMaking(0);
                    }

                    if (it1.ItemReturn().type == Item.TYPE.Box && it1.slot.Count >= 1)
                    {
                        //img.gameObject.SetActive(true);
                        it1.ItemUse();
                        detail_text1.text = "수수께끼상자" + it1.slot.Count.ToString() + "/1";
                        //bb.gameObject.SetActive(false);
                        //b1 = true;
                        Game.Network.NetWork.SendCompleteMaking(0);
                    }


                }

                break;

            case 2:
                //else if (select_list_num == 2)
                {
                    //img2.gameObject.SetActive(true);
                    //bb2.gameObject.SetActive(false);
                    //b2 = true;
                    if (it0.ItemReturn().type == Item.TYPE.Crystal && it0.slot.Count >= 1)
                    {
                        //img2.gameObject.SetActive(true);
                        it0.ItemUse();
                        detail_text1.text = "삼각플라스크" + it0.slot.Count.ToString() + "/1";
                        //bb2.gameObject.SetActive(false);
                        //b2 = true;
                        Game.Network.NetWork.SendCompleteMaking(1);
                    }

                    if (it1.ItemReturn().type == Item.TYPE.Crystal && it1.slot.Count >= 1)
                    {
                        //img2.gameObject.SetActive(true);
                        it1.ItemUse();
                        detail_text1.text = "삼각플라스크" + it1.slot.Count.ToString() + "/1";
                        //bb2.gameObject.SetActive(false);
                        //b2 = true;
                        Game.Network.NetWork.SendCompleteMaking(1);
                    }

                }
                break;

            case 3:
                //img2.gameObject.SetActive(true);
                   //bb2.gameObject.SetActive(false);
                   //b2 = true;
                if (it0.ItemReturn().type == Item.TYPE.Box && it0.slot.Count >= 1)
                {
                    //img3.gameObject.SetActive(true);
                    it0.ItemUse();
                    detail_text1.text = "수수께끼상자" + it0.slot.Count.ToString() + "/1";
                   // bb3.gameObject.SetActive(false);
                   // b3 = true;
                    Game.Network.NetWork.SendCompleteMaking(2);
                }

                if (it0.ItemReturn().type == Item.TYPE.Crystal && it0.slot.Count >= 1)
                {
                    //img3.gameObject.SetActive(true);
                    it0.ItemUse();
                    detail_text1.text = "삼각플라스크" + it0.slot.Count.ToString() + "/1";
                    //bb3.gameObject.SetActive(false);
                   // b3 = true;
                    Game.Network.NetWork.SendCompleteMaking(2);
                }

                if (it1.ItemReturn().type == Item.TYPE.Box && it1.slot.Count >= 1)
                {
                    //img3.gameObject.SetActive(true);
                    it1.ItemUse();
                    detail_text1.text = "수수께끼상자" + it1.slot.Count.ToString() + "/1";
                  //  bb3.gameObject.SetActive(false);
                   // b3 = true;
                    Game.Network.NetWork.SendCompleteMaking(2);
                }

                if (it1.ItemReturn().type == Item.TYPE.Crystal && it1.slot.Count >= 1)
                {
                  //  img3.gameObject.SetActive(true);
                    it1.ItemUse();
                    detail_text1.text = "삼각플라스크" + it1.slot.Count.ToString() + "/1";
                  //  bb3.gameObject.SetActive(false);
                  //  b3 = true;
                    Game.Network.NetWork.SendCompleteMaking(2);
                }
                break;

            case 4:
                //img2.gameObject.SetActive(true);
                //bb2.gameObject.SetActive(false);
                //b2 = true;
                if (it0.ItemReturn().type == Item.TYPE.Box && it0.slot.Count >= 1)
                {
                    //img4.gameObject.SetActive(true);
                    it0.ItemUse();
                    detail_text1.text = "수수께끼상자" + it0.slot.Count.ToString() + "/1";
                   // bb4.gameObject.SetActive(false);
                   // b4 = true;
                    Game.Network.NetWork.SendCompleteMaking(3);
                }
                if (it0.ItemReturn().type == Item.TYPE.Crystal && it0.slot.Count >= 1)
                {
                    //img4.gameObject.SetActive(true);
                    it0.ItemUse();
                    detail_text1.text = "삼각플라스크" + it0.slot.Count.ToString() + "/1";
                  //  bb4.gameObject.SetActive(false);
                  //  b4 = true;
                    Game.Network.NetWork.SendCompleteMaking(3);
                }
                if (it1.ItemReturn().type == Item.TYPE.Crystal && it1.slot.Count >= 1)
                {
                  //  img4.gameObject.SetActive(true);
                    it1.ItemUse();
                    detail_text1.text = "삼각플라스크" + it1.slot.Count.ToString() + "/1";
                  //  bb4.gameObject.SetActive(false);
                   // b4 = true;
                    Game.Network.NetWork.SendCompleteMaking(3);
                }
                if (it1.ItemReturn().type == Item.TYPE.Box && it1.slot.Count >= 1)
                {
                  //  img4.gameObject.SetActive(true);
                    it1.ItemUse();
                    detail_text1.text = "수수께끼상자" + it1.slot.Count.ToString() + "/1";
                  //  bb4.gameObject.SetActive(false);
                  //  b4 = true;
                    Game.Network.NetWork.SendCompleteMaking(3);
                }
                break;
            case 5:
                //img2.gameObject.SetActive(true);
                //bb2.gameObject.SetActive(false);
                //b2 = true;
                
                if (b1 && b2 && b3 && b4)
                {
                    //img5.gameObject.SetActive(true);
                    //detail_text1.text = it1.slot.Count.ToString() + "/1";
                    //bb5.gameObject.SetActive(false);
                    //b5 = true;
                    Game.Network.NetWork.SendCompleteMaking(4);
                }
                    break;
            default:
                break;
        }

        if (b5)
        {
            ending.gameObject.SetActive(true);
            
        }
    }

    public void Ending()
    {
        SceneManager.LoadScene(3);
    }
}
