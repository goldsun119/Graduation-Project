using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Dead_window : MonoBehaviour
{
    public GameObject text;
    public GameObject button;
    public float FadeTime = 2f; // Fade효과 재생시간
    Image fadeImg;
    float start;
    float end;

    float time = 0f;
    bool isPlaying = false;

    void Awake()
    {
        fadeImg = GetComponent<Image>();
        //InStartFadeAnim();
    }
    void Update()
    {
        string a = "Player(" + Game.Network.NetWork.Client_id.ToString() + ")";
        GameObject playerObj = GameObject.Find("Players").transform.Find(a).gameObject;
        PlayerStatus ps = playerObj.GetComponent<PlayerStatus>();

        if (ps.isdie == true)
            OutStartFadeAnim();
    }

    public void OutStartFadeAnim()
    {
        if (isPlaying == true) //중복재생방지
        {
            text.gameObject.SetActive(true);
            button.gameObject.SetActive(true);
            return;
        }
        start = 0.0f;
        end = 0.7f;
    
        StartCoroutine("fadeoutplay");    //코루틴 실행
    }
    public void InStartFadeAnim()
    {
        if (isPlaying == true) //중복재생방지
        {
            return;
        }

        StartCoroutine("fadeIntanim");
    }

    IEnumerator fadeoutplay()
    {
        isPlaying = true;
        
        Color fadecolor = fadeImg.color;
        time = 0f;
        fadecolor.a = Mathf.Lerp(start, end, time);
        while (fadecolor.a < 0.7f)
        {
            time += Time.deltaTime / FadeTime;
            fadecolor.a = Mathf.Lerp(start, end, time);
            fadeImg.color = fadecolor;

            yield return null;
        }
        //isPlaying = false;
    }
}
