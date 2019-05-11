using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerAnimation : MonoBehaviour
{
    PlayerStatus player;
    Animator animator;

    // Start is called before the first frame update
    void Start()
    {
        player = GetComponent<PlayerStatus>();
        animator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        MoveAni();
    }

    void MoveAni()
    {
        // 걷기
        if (player.Ani_State_Walk_Run == PlayerStatus.ANI_TYPE.IDEL) animator.SetInteger("anitype", 0);
        else if (player.Ani_State_Walk_Run == PlayerStatus.ANI_TYPE.WALK) animator.SetInteger("anitype", 1);
        else if (player.Ani_State_Walk_Run == PlayerStatus.ANI_TYPE.RUN) animator.SetInteger("anitype", 2);
        else animator.SetInteger("anitype", 0);


    }
}
