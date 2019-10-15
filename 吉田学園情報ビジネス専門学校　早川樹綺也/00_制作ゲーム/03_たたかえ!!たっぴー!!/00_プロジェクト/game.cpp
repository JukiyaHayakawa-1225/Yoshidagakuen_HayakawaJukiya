//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "map.h"
#include "modelcreate.h"
#include "camera.h"
#include "cameraManager.h"
#include "textureManager.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "input.h"
#include "fade.h"
#include "UI.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "respawn.h"
#include "item.h"
#include "itemCylinder.h"
#include "river.h"
#include "boxCollider.h"
#include "scene2D.h"
#include "number.h"
#include "server.h"
#include "sound.h"
#include "charaselect.h"
#include "effectManager.h"
#include "characterManager.h"
#include "playerManager.h"
#include "blossoms.h"
#include "hinaarare.h"
#include "mist.h"
#include "power_map.h"
#include "title.h"
#include "result.h"
#include "debugproc.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define GAME_DEBUG      // �錾���f�o�b�O�R�}���h�K�p
#define GAME_MAPEVENT   // �錾���}�b�v�C�x���g�K�p

// �}�b�v�X�V���ɏ������I�u�W�F�N�g(����p�̃}�N���Ȃ̂ł����ɒǉ�)
#define GAME_DELETE_OBJECT (pScene->GetObjType() == CScene::OBJTYPE_ITEM || pScene->GetObjType() == CScene::OBJTYPE_BULLET || pScene->GetObjType() == CScene::OBJTYPE_PARTICLE || pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER || pScene->GetObjType() == CScene::OBJTYPE_EMITTER || pScene->GetObjType() == CScene::OBJTYPE_BLOSSOMS || pScene->GetObjType() == CScene::OBJTYPE_ITEMCYLINDER)

#ifdef GAME_DEBUG
#define GAME_DEBUG_STARTMAP                          (CGame::MAPTYPE_HINAMATSURI)
#endif

#define GAME_SYSTEM_FILENAME                         "data/TEXT/MODE/game.ini"    // �������Ɏg�p����V�X�e���t�@�C����
#define GAME_STAGEDISP_TIME                          (120)                        // �X�e�[�W�ԍ��\����ԂɂȂ��Ă���ʏ��Ԃɐ؂�ւ��܂ł̎���
#define GAME_RESULT_TIMING                           (180)                        // �I����ԂɂȂ��Ă��烊�U���g��Ԃɐ؂�ւ��܂ł̎���
#define GAME_NEXTMODE_TIMING                         (180)                        // ���U���g��ԂɂȂ��Ă��玟�̃��[�h�ɐ؂�ւ��܂ł̎���
#define GAME_NEXTSCORE_TIMING                        (30)                         // �Q�[�������U���g�Ŏ��̃X�R�A���o���܂ł̎���
#define GAME_SCOREUP_TIMING                          (10)                         // �Q�[�������U���g�ŃX�R�A�A�b�v������܂ł̎���
#define GAME_GAMEOVER_TIMING                         (180)                        // �Q�[���I�[�o�[��ԂɂȂ��Ă��玟�̃��[�h�ɐ؂�ւ��܂ł̎���
#define GAME_GAMEOVER_POSUP                          (8.0f)                       // �Q�[���I�[�o�[���S���ǂꂭ�炢�グ�邩
#define GAME_GAMEOVER_POSYMAX                        (10.0f)                      // �Q�[���I�[�o�[�|���S����Y���W�ő�l(�|���S���̍����ɉ����Ĕ���)
#define GAME_NUMBER_INTERVAL_CUT                     (0.8f)                       // �����̊Ԋu�̔{��(�|���S���̕�������ŏ�Z����)
#define GAME_PLAYER_STOCK_INI                        (3)                          // �v���C���[�̎c�@�������l
#define GAME_PLAYER_RESPAWN_TIMING                   (180)                        // �v���C���[�����X�|�[������܂ł̎���
#define GAME_MAPSTAGE_MAX                            (3)                          // �X�e�[�W�̎�ނ��Ƃ̐�
#define GAME_ITEM_SCORE                              (500)                        // �A�C�e���̃X�R�A�̗�
#define GAME_BONUS_SCORE                             (1000)                       // �{�[�i�X�X�R�A�̗�
#define GAME_STAGEPOLYGON_PRIORITY                   (8)                          // �X�e�[�W�|���S���̕`��D�揇��
#define GAME_PLAYER_SPAWN_EFFECT_IDX                 (11)                         // �v���C���[�����X�|�[������Ƃ��̃G�t�F�N�g
#define GAME_MAPEVENT_RANDOM                         (600)                        // �}�b�v�C�x���g���N���������_���]���l
#define GAME_SE_FANFARE_IDX                          (5)                          // �Q�[���ŃX�e�[�W���n�܂�Ƃ��̉��ԍ�
#define GAME_SE_PLAYER_RESPAWN_IDX                   (17)                         // �v���C���[�����X�|�[���������̉��ԍ�
#define GAME_SE_PAUSE_OPEN_IDX                       (11)                         // �|�[�Y��ʂ��J�����Ƃ��̉��ԍ�
#define GAME_SE_GAMERESULT_SCOREUP_IDX               (13)                         // �Q�[�����U���g�ŃX�R�A�����Z�����Ƃ��̉��ԍ�
#define GAME_SE_BONUS_IDX                            (14)                         // �{�[�i�X��t�^�������̉��ԍ�


// �A�C�e���C�x���g�p�}�N��
#define GAME_ITEMEVENT_GRENADESHAKE_TIME             (10)                         // �O���l�[�h����������ɃJ�������ǂꂭ�炢�h�炵�����邩
#define GAME_ITEMEVENT_GRENADESHAKE_VALUE            (40.0f)                      // �O���l�[�h����������ɃJ�������ǂꂭ�炢�h�炷��
#define GAME_ITEMEVENT_GRENADESHAKE_CUT              (3.0f)                       // �J�����̗h����ǂꂭ�炢���炵�Ă�����
#define GAME_ITEMEVENT_ENEMYSTOPTIME                 (300)                        // �G���ǂꂭ�炢�~�߂Ă�����
#define GAME_ITEMEVENT_SE_STAR                       (19)                         // �X�^�[�A�C�e���C�x���g�̉��ԍ�
#define GAME_ITEMEVENT_SE_GRENADE                    (20)                         // �O���l�[�h�A�C�e���C�x���g�̉��ԍ�
#define GAME_ITEMEVENT_SE_1UP                        (21)                         // 1UP�A�C�e���C�x���g�̉��ԍ�
#define GAME_ITEMEVENT_SE_SCOOP                      (22)                         // �X�R�b�v�A�C�e���C�x���g�̉��ԍ�
#define GAME_ITEMEVENT_SE_CLOCK                      (23)                         // ���v�A�C�e���C�x���g�̉��ԍ�
#define GAME_ITEMEVENT_SE_HELMET                     (24)                         // �w�����b�g�A�C�e���C�x���g�̉��ԍ�

// �X�e�[�W�w�i�|���S���������p
#define GAME_STAGEBG_POS_INI                         (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_STAGEBG_COL_INI                         (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f))
#define GAME_STAGEBG_WIDTH_INI                       (SCREEN_WIDTH / 2.0f)
#define GAME_STAGEBG_HEIGHT_INI                      (SCREEN_HEIGHT / 2.0f)

// �X�e�[�W���S�|���S���������p
#define GAME_STAGELOGO_POS_INI                       (D3DXVECTOR3(560.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_STAGELOGO_COL_INI                       (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))
#define GAME_STAGELOGO_WIDTH_INI                     (170.0f)
#define GAME_STAGELOGO_HEIGHT_INI                    (60.0f)

// �X�e�[�W�ԍ��\���p�|���S���������p
#define GAME_STAGENUMBER_POS_INI                     (D3DXVECTOR3(830.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_STAGENUMBER_COL_INI                     (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_STAGENUMBER_WIDTH_INI                   (40.0f)
#define GAME_STAGENUMBER_HEIGHT_INI                  (40.0f)

// �Q�[���I�[�o�[���S�|���S���������p
#define GAME_GAMEOVERLOGO_POS_INI                    (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 900.0f, 0.0f))
#define GAME_GAMEOVERLOGO_COL_INI                    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMEOVERLOGO_WIDTH_INI                  (80.0f)
#define GAME_GAMEOVERLOGO_HEIGHT_INI                 (80.0f)
#define GAME_GAMEOVERLOGO_PRIORITY                   (8)

// �}�b�v�C�x���g�p
#define GAME_MAPEVENT_MAX                            (3)
#define GAME_MAPEVENT_STARTTIME                      (200)
#define GAME_MAPEVENT_SOUNDIDX                       (25)
#define GAME_MAPEVENT_STARTLOGO_WIDTH                (300.0f)
#define GAME_MAPEVENT_STARTLOGO_HEIGHT               (90.0f)
#define GAME_MAPEVENT_STARTLOGO_POS                  (D3DXVECTOR3(SCREEN_WIDTH + 100.0f, GAME_MAPEVENT_STARTLOGO_HEIGHT - 30.0f, 0.0f))
#define GAME_MAPEVENT_STARTLOGO_COL                  (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_MAPEVENT_STARTLOGO_PRIORITY             (6)
#define GAME_MAPEVENT_STARTLOGO_TEXIDX               (29)
#define GAME_MAPEVENT_STARTLOGO_TEXU_SPLIT           (2)
#define GAME_MAPEVENT_STARTLOGO_TEXV_SPLIT           (3)
#define GAME_MAPEVENT_STARTLOGO_MOVE                 (-9.0f)

// ���̉Ԃт�f�[�^�������p
#define GAME_BLOSSOMS_POS_INI                        (D3DXVECTOR3(SCREEN_WIDTH - UI_BG_WIDTH_INI + 100.0f, -100.0f, 0.0f))
#define GAME_BLOSSOMS_COL_INI                        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_BLOSSOMS_TIME_INI                       (300)
#define GAME_BLOSSOMS_LAP_INI                        (3)
#define GAME_BLOSSOMS_MOVEX_MAX_INI                  (15)
#define GAME_BLOSSOMS_MOVEX_MIN_INI                  (5)
#define GAME_BLOSSOMS_MOVEY_MAX_INI                  (10)
#define GAME_BLOSSOMS_MOVEY_MIN_INI                  (2)
#define GAME_BLOSSOMS_WIDTH_MAX_INI                  (20)
#define GAME_BLOSSOMS_WIDTH_MIN_INI                  (10)
#define GAME_BLOSSOMS_HEIGHT_MAX_INI                 (20)
#define GAME_BLOSSOMS_HEIGHT_MIN_INI                 (10)
#define GAME_BLOSSOMS_ANGLESPEED_MAX_INI             (5)
#define GAME_BLOSSOMS_ANGLESPEED_MIN_INI             (0)
#define GAME_BLOSSOMS_ROTSPEED_MAX_INI               (5)
#define GAME_BLOSSOMS_ROTSPEED_MIN_INI               (0)
#define GAME_BLOSSOMS_BGMIDX                         (28)

// �A�C�e���h���b�v�C�x���g�p
#define GAME_DROPITEM_POS_Y                          (1200.0f)
#define GAME_DROPITEM_CYLINDER_MOVE_Y                (-14.0f)
#define GAME_DROPITEM_ITEM_MOVE_Y                    (-10.0f)
#define GAME_DROPITEM_CYLINDER_COL                   (D3DXCOLOR(0.3f,0.3f,1.0f,1.0f))
#define GAME_DROPITEM_CYLINDER_HEIGHT                (3600.0f)
#define GAME_DROPITEM_CYLINDER_RADIUS                (50.0f)
#define GAME_DROPITEM_CYLINDER_XBLOCK                (20)
#define GAME_DROPITEM_CYLINDER_YBLOCK                (1)
#define GAME_DROPITEM_CYLINDER_PRIORITY              (7)
#define GAME_DROPITEM_EFFECT_IDX                     (14)
#define GAME_DROPITEM_EFFECT_APPEAR                  (7)
#define GAME_DROPITEM_CYLINDER_SE_IDX                (29)
#define GAME_DROPITEM_ITEM_SE_IDX                    (30)

// �ЂȂ����f�[�^�������p
#define GAME_HINAARARE_POSY                          (800.0f)
#define GAME_HINAARARE_MODELIDX                      (0)
#define GAME_HINAARARE_TIME                          (300)
#define GAME_HINAARARE_APPEAR                        (60)
#define GAME_HINAARARE_CAMERA_SHAKE                  (3.0f)
#define GAME_HINAARARE_MODEL_NUM                     (4)
#define GAME_HINAARARE_BGMIDX                        (31)

// �Q�[�������U���g�p�w�i�|���S���������p
#define GAME_GAMERESULT_BG_POS_INI                   (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_GAMERESULT_BG_COL_INI                   (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f))
#define GAME_GAMERESULT_BG_WIDTH_INI                 (SCREEN_WIDTH / 2.0f)
#define GAME_GAMERESULT_BG_HEIGHT_INI                (SCREEN_HEIGHT / 2.0f)
#define GAME_GAMERESULT_BG_PRIORITY                  (8)

// �Q�[�������U���g�p�n�C�X�R�A���S�|���S���������p
#define GAME_GAMERESULT_HIGHSCORELOGO_POS_INI        (D3DXVECTOR3(500.0f, 25.0f, 0.0f))
#define GAME_GAMERESULT_HIGHSCORELOGO_COL_INI        (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))
#define GAME_GAMERESULT_HIGHSCORELOGO_WIDTH_INI      (100.0f)
#define GAME_GAMERESULT_HIGHSCORELOGO_HEIGHT_INI     (25.0f)
#define GAME_GAMERESULT_HIGHSCORELOGO_PRIORITY       (8)
#define GAME_GAMERESULT_HIGHSCORELOGO_TEXIDX         (30)

// �Q�[�������U���g�p�n�C�X�R�A�����|���S���������p
#define GAME_GAMERESULT_HIGHSCORENUMBER_POS_INI      (D3DXVECTOR3(830.0f, 25.0f, 0.0f))
#define GAME_GAMERESULT_HIGHSCORENUMBER_COL_INI      (D3DXCOLOR(1.0f,0.73f,0.31f,1.0f))
#define GAME_GAMERESULT_HIGHSCORENUMBER_WIDTH_INI    (15.0f)
#define GAME_GAMERESULT_HIGHSCORENUMBER_HEIGHT_INI   (15.0f)
#define GAME_GAMERESULT_HIGHSCORENUMBER_INTERVAL_INI (D3DXVECTOR3(-GAME_GAMERESULT_HIGHSCORENUMBER_WIDTH_INI - 14.0f, 0.0f, 0.0f))
#define GAME_GAMERESULT_HIGHSCORENUMBER_TEXIDX       (28)
#define GAME_GAMERESULT_HIGHSCORENUMBER_PRIORITY     (8)

// �Q�[�������U���g�p�X�e�[�W���S�|���S���������p
#define GAME_GAMERESULT_STAGELOGO_POS_INI            (D3DXVECTOR3(620.0f, 85.0f, 0.0f))
#define GAME_GAMERESULT_STAGELOGO_COL_INI            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_STAGELOGO_WIDTH_INI          (100.0f)
#define GAME_GAMERESULT_STAGELOGO_HEIGHT_INI         (25.0f)
#define GAME_GAMERESULT_STAGELOGO_PRIORITY           (8)
#define GAME_GAMERESULT_STAGELOGO_TEXIDX             (1)

// �Q�[�������U���g�p�X�e�[�W�ԍ��|���S���������p
#define GAME_GAMERESULT_STAGENUMBER_POS_INI          (D3DXVECTOR3(740.0f, 85.0f, 0.0f))
#define GAME_GAMERESULT_STAGENUMBER_COL_INI          (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_STAGENUMBER_WIDTH_INI        (20.0f)
#define GAME_GAMERESULT_STAGENUMBER_HEIGHT_INI       (20.0f)
#define GAME_GAMERESULT_STAGENUMBER_PRIORITY         (8)

// �Q�[�������U���g�p�v���C���[���S�|���S���������p
#define GAME_GAMERESULT_PLAYERLOGO_POS_INI           (D3DXVECTOR3(SCREEN_WIDTH / 4.0f, 135.0f, 0.0f))
#define GAME_GAMERESULT_PLAYERLOGO_COL_INI           (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))
#define GAME_GAMERESULT_PLAYERLOGO_WIDTH_INI         (120.0f)
#define GAME_GAMERESULT_PLAYERLOGO_HEIGHT_INI        (17.0f)
#define GAME_GAMERESULT_PLAYERLOGO_PRIORITY          (8)
#define GAME_GAMERESULT_PLAYERLOGO_TEXIDX            (13)

// �Q�[�������U���g�p�v���C���[�X�R�A�|���S���������p
#define GAME_GAMERESULT_PLAYERNUMBER_POS_INI         (D3DXVECTOR3(380.0f, 175.0f, 0.0f))
#define GAME_GAMERESULT_PLAYERNUMBER_COL_INI         (D3DXCOLOR(1.0f,0.73f,0.31f,1.0f))
#define GAME_GAMERESULT_PLAYERNUMBER_WIDTH_INI       (15.0f)
#define GAME_GAMERESULT_PLAYERNUMBER_HEIGHT_INI      (15.0f)
#define GAME_GAMERESULT_PLAYERNUMBER_INTERVAL_INI    (D3DXVECTOR3(-GAME_GAMERESULT_PLAYERNUMBER_WIDTH_INI - 14.0f, 0.0f, 0.0f))
#define GAME_GAMERESULT_PLAYERNUMBER_PRIORITY        (8)

// �Q�[�������U���g�p�G���|���S���������p
#define GAME_GAMERESULT_ENEMY_INTERVAL_INI           (80.0f)
#define GAME_GAMERESULT_ENEMYARROW_POS_INI           (D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 50.0f, 250.0f, 0.0f))
#define GAME_GAMERESULT_ENEMYARROW_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_ENEMYARROW_WIDTH_INI         (15.0f)
#define GAME_GAMERESULT_ENEMYARROW_HEIGHT_INI        (15.0f)
#define GAME_GAMERESULT_ENEMYARROW_INTERVAL_INI      (D3DXVECTOR3(50.0f, GAME_GAMERESULT_ENEMY_INTERVAL_INI, 0.0f))
#define GAME_GAMERESULT_ENEMYARROW_TEXIDX            (9)
#define GAME_GAMERESULT_ENEMYARROW_PRIORITY          (8)

// �Q�[�������U���g�p�G�|�C���g���S�|���S���������p
#define GAME_GAMERESULT_ENEMYPOINTLOGO_POS_INI       (D3DXVECTOR3(430.0f, 250.0f, 0.0f))
#define GAME_GAMERESULT_ENEMYPOINTLOGO_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_ENEMYPOINTLOGO_WIDTH_INI     (30.0f)
#define GAME_GAMERESULT_ENEMYPOINTLOGO_HEIGHT_INI    (20.0f)
#define GAME_GAMERESULT_ENEMYPOINTLOGO_INTERVAL_INI  (D3DXVECTOR3(210.0f, GAME_GAMERESULT_ENEMY_INTERVAL_INI, 0.0f))
#define GAME_GAMERESULT_ENEMYPOINTLOGO_TEXIDX        (11)
#define GAME_GAMERESULT_ENEMYPOINTLOGO_PRIORITY      (8)

// �Q�[�������U���g�p�G�A�C�R���|���S���������p
#define GAME_GAMERESULT_ENEMYICON_POS_INI            (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 250.0f, 0.0f))
#define GAME_GAMERESULT_ENEMYICON_COL_INI            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_ENEMYICON_WIDTH_INI          (20.0f)
#define GAME_GAMERESULT_ENEMYICON_HEIGHT_INI         (20.0f)
#define GAME_GAMERESULT_ENEMYICON_INTERVAL_INI       (D3DXVECTOR3(0.0f, GAME_GAMERESULT_ENEMY_INTERVAL_INI, 0.0f))
#define GAME_GAMERESULT_ENEMYICON_PRIORITY           (8)
#define GAME_GAMERESULT_ENEMYICON_TEXIDX             (15)

// �Q�[�������U���g�p�G�X�R�A�|���S���|���S���������p
#define GAME_GAMERESULT_ENEMYSCORE_POS_INI           (D3DXVECTOR3(330.0f, 250.0f, 0.0f))
#define GAME_GAMERESULT_ENEMYSCORE_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_ENEMYSCORE_WIDTH_INI         (15.0f)
#define GAME_GAMERESULT_ENEMYSCORE_HEIGHT_INI        (15.0f)
#define GAME_GAMERESULT_ENEMYSCORE_INTERVAL_INI      (D3DXVECTOR3(-30.0f, GAME_GAMERESULT_ENEMY_INTERVAL_INI, 0.0f))
#define GAME_GAMERESULT_ENEMYSCORE_PRIORITY          (8)

// �Q�[�������U���g�p�G���j���|���S���|���S���������p
#define GAME_GAMERESULT_ENEMYBREAK_POS_INI           (D3DXVECTOR3(540.0f, 250.0f, 0.0f))
#define GAME_GAMERESULT_ENEMYBREAK_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_ENEMYBREAK_WIDTH_INI         (15.0f)
#define GAME_GAMERESULT_ENEMYBREAK_HEIGHT_INI        (15.0f)
#define GAME_GAMERESULT_ENEMYBREAK_INTERVAL_INI      (D3DXVECTOR3(-30.0f, GAME_GAMERESULT_ENEMY_INTERVAL_INI, 0.0f))
#define GAME_GAMERESULT_ENEMYBREAK_PRIORITY          (8)

// �Q�[�������U���g�p�g�[�^�����S�|���S���������p
#define GAME_GAMERESULT_TOTALLOGO_POS_INI            (D3DXVECTOR3(370.0f, 580.0f, 0.0f))
#define GAME_GAMERESULT_TOTALLOGO_COL_INI            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_TOTALLOGO_WIDTH_INI          (80.0f)
#define GAME_GAMERESULT_TOTALLOGO_HEIGHT_INI         (15.0f)
#define GAME_GAMERESULT_TOTALLOGO_PRIORITY           (8)
#define GAME_GAMERESULT_TOTALLOGO_TEXIDX             (12)

// �Q�[�������U���g�p�g�[�^�����C���|���S���������p
#define GAME_GAMERESULT_TOTALLINE_POS_INI            (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 545.0f, 0.0f))
#define GAME_GAMERESULT_TOTALLINE_COL_INI            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_TOTALLINE_WIDTH_INI          (180.0f)
#define GAME_GAMERESULT_TOTALLINE_HEIGHT_INI         (3.2f)
#define GAME_GAMERESULT_TOTALLINE_PRIORITY           (8)

// �Q�[�������U���g�p�g�[�^���X�R�A�|���S���������p
#define GAME_GAMERESULT_TOTALSCORE_POS_INI           (D3DXVECTOR3(540.0f, 580.0f, 0.0f))
#define GAME_GAMERESULT_TOTALSCORE_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_TOTALSCORE_WIDTH_INI         (15.0f)
#define GAME_GAMERESULT_TOTALSCORE_HEIGHT_INI        (15.0f)
#define GAME_GAMERESULT_TOTALSCORE_INTERVAL_INI      (D3DXVECTOR3(-30.0f, 0.0f, 0.0f))
#define GAME_GAMERESULT_TOTALSCORE_PRIORITY          (8)

// �Q�[�������U���g�p�{�[�i�X�X�R�A���S�|���S���������p
#define GAME_GAMERESULT_BONUS_ADDPOS                 (SCREEN_WIDTH / 2.0f)
#define GAME_GAMERESULT_BONUSSCORELOGO_POS_INI       (D3DXVECTOR3(240.0f, 630.0f, 0.0f))
#define GAME_GAMERESULT_BONUSSCORELOGO_COL_INI       (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))
#define GAME_GAMERESULT_BONUSSCORELOGO_WIDTH_INI     (120.0f)
#define GAME_GAMERESULT_BONUSSCORELOGO_HEIGHT_INI    (17.0f)
#define GAME_GAMERESULT_BONUSSCORELOGO_TEXIDX        (25)
#define GAME_GAMERESULT_BONUSSCORELOGO_PRIORITY      (8)

// �Q�[�������U���g�p�{�[�i�X�|�C���g���S�|���S���������p
#define GAME_GAMERESULT_BONUSPOINTLOGO_POS_INI       (D3DXVECTOR3(430.0f, 680.0f, 0.0f))
#define GAME_GAMERESULT_BONUSPOINTLOGO_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_BONUSPOINTLOGO_WIDTH_INI     (30.0f)
#define GAME_GAMERESULT_BONUSPOINTLOGO_HEIGHT_INI    (20.0f)
#define GAME_GAMERESULT_BONUSPOINTLOGO_TEXIDX        (11)
#define GAME_GAMERESULT_BONUSPOINTLOGO_PRIORITY      (8)

// �Q�[�������U���g�p�{�[�i�X�X�R�A���S�|���S���������p
#define GAME_GAMERESULT_BONUSSCORE_POS_INI           (D3DXVECTOR3(320.0f, 680.0f, 0.0f))
#define GAME_GAMERESULT_BONUSSCORE_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_BONUSSCORE_WIDTH_INI         (15.0f)
#define GAME_GAMERESULT_BONUSSCORE_HEIGHT_INI        (15.0f)
#define GAME_GAMERESULT_BONUSSCORE_INTERVAL_INI      (D3DXVECTOR3(-30.0f, 0.0f, 0.0f))
#define GAME_GAMERESULT_BONUSSCORE_PRIORITY          (8)

// �|�[�Y�����v���C���[�ԍ��\���p�|���S���������p
#define GAME_PAUSE_PLAYERNUMBER_POS_INI              (D3DXVECTOR3(120.0f, 60.0f, 0.0f))
#define GAME_PAUSE_PLAYERNUMBER_COL_INI              (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_PAUSE_PLAYERNUMBER_WIDTH_INI            (120.0f)
#define GAME_PAUSE_PLAYERNUMBER_HEIGHT_INI           (60.0f)
#define GAME_PAUSE_PLAYERNUMBER_PRIORITY             (8)
#define GAME_PAUSE_PLAYERNUMBER_TEXIDX               (26)

// �|�[�Y���Ă܂��|���S���������p
#define GAME_PAUSE_NOT_POS_INI                       (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_PAUSE_NOT_COL_INI                       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_PAUSE_NOT_WIDTH_INI                     (120.0f)
#define GAME_PAUSE_NOT_HEIGHT_INI                    (60.0f)
#define GAME_PAUSE_NOT_PRIORITY                      (8)
#define GAME_PAUSE_NOT_TEXIDX                        (28)

// �|�[�Y���Ă܂����w�i�|���S���������p
#define GAME_PAUSE_NOT_BLACKBG_COL_INI               (D3DXCOLOR(0.0f,0.0f,0.0f,0.5f))
#define GAME_PAUSE_NOT_BLACKBG_PRIORITY              (8)


// �N���[���Ƀ|���S���������p
#define GAME_CREAMMIST_POS_INI                       (D3DXVECTOR3(SCREEN_WIDTH / 2.0f - UI_BG_WIDTH_INI, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_CREAMMIST_COL_INI                       (D3DXCOLOR(1.0f,1.0f,1.0f,0.3f))
#define GAME_CREAMMIST_WIDTH_INI                     (SCREEN_WIDTH / 2.0f)
#define GAME_CREAMMIST_HEIGHT_INI                    (SCREEN_HEIGHT / 2.0f)
#define GAME_CREAMMIST_ALPHA_UP                      (0.3f)
#define GAME_CREAMMIST_ALPHA_CUT                     (0.02f)
#define GAME_CREAMMIST_ALPHA_MAX                     (0.8f)
#define GAME_CREAMMIST_APPAER_TIME                   (360)
#define GAME_CREAMMIST_PRIORITY                      (5)
#define GAME_CREAMMIST_TEXIDX                        (31)

// �l�ǂݍ��ݗp�̃p�X
// �e�N�X�`���p
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// ���f���p
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "

// �}�b�v�p
#define NUM_MAP "NUM_MAP = "

// �A�C�e���C�x���g�p
#define ITEMEVENT_FILENAME "ITEMEVENT_FILENAME = "
#define SHAKESET "SHAKESET"
#define END_SHAKESET "END_SHAKESET"
#define TIME "TIME = "
#define VALUE "VALUE = "
#define CUT "CUT = "
#define ENEMY_STOP "ENEMY_STOP = "

// �G�t�F�N�g�p
#define EFFECT_FILENAME "EFFECT_FILENAME = "

// �e�p
#define BULLET_MODEL_IDX "BULLET_MODEL_IDX = "

// �A�C�e���p
#define ITEM_MODEL_IDX "ITEM_MODEL_IDX = "

// �v���C���[�f�[�^�p
#define PLAYERDATA_FILENAME "PLAYERDATA_FILENAME = "
#define PLAYER_STOCK_INI "PLAYER_STOCK_INI = "
#define PLAYER_STOCK_MAX "PLAYER_STOCK_MAX = "

// �G�f�[�^�p
#define ENEMY_FILENAME "ENEMY_FILENAME = "

// �w�i�|���S���p
#define BGPOLYGONSET "BGPOLYGONSET"
#define END_BGPOLYGONSET "END_BGPOLYGONSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

// �����e�N�X�`���p
#define NUMBER_TEX_IDX "NUMBER_TEX_IDX = "

// �X�e�[�W�w�i�|���S���p
#define STAGEBGSET "STAGEBGSET"
#define END_STAGEBGSET "END_STAGEBGSET"

// �X�e�[�W���S�|���S���p
#define STAGELOGOSET "STAGELOGOSET"
#define END_STAGELOGOSET "END_STAGELOGOSET"

// �X�e�[�W�ԍ��|���S���p
#define STAGENUMBERSET "STAGENUMBERSET"
#define END_STAGENUMBERSET "END_STAGENUMBERSET"

// �Q�[���I�[�o�[���S�|���S���p
#define GAMEOVERPOLYGONSET "GAMEOVERPOLYGONSET"
#define END_GAMEOVERPOLYGONSET "END_GAMEOVERPOLYGONSET"

// �ЂȍՂ�C�x���g�p
#define HINAMATSURI_FILENAME "HINAMATSURI_FILENAME = "
#define EVENT_RANDOM "EVENT_RANDOM = "

// ���̉Ԃт�f�[�^�p
#define CHERRYBLOSSOMSSET "CHERRYBLOSSOMSSET"
#define END_CHERRYBLOSSOMSSET "END_CHERRYBLOSSOMSSET"
#define TIME "TIME = "
#define LAP "LAP = "
#define MOVEX "MOVEX = "
#define MOVEY "MOVEY = "
#define ANGLESPEED "ANGLESPEED = "
#define ROTSPEED "ROTSPEED = "

// �ЂȂ����f�[�^�p
#define HINAARARESET "HINAARARESET"
#define END_HINAARARESET "END_HINAARARESET"
#define MODEL_IDX "MODEL_IDX = "
#define APPEAR "APPEAR = "

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �Q�[���̃R���X�g���N�^
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// �Q�[���̃f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// �Q�[���̐�������
//=============================================================================
CGame *CGame::Create()
{
	CGame *pGame = NULL;
	if (pGame == NULL)
	{
		pGame = new CGame;
		if (pGame != NULL)
		{
			pGame->Init();
		}
	}
	return pGame;
}

//=============================================================================
// �Q�[���̏���������
//=============================================================================
HRESULT CGame::Init(void)
{
	// �ϐ�������������
	ClearVariable();

	// �}�b�v�̔ԍ���ݒ肷��
#ifdef GAME_DEBUG
	CCharaSelect::SetStageType(GAME_DEBUG_STARTMAP);
#endif
	m_nMapIdx = CCharaSelect::GetStageType() * GAME_MAPSTAGE_MAX;

	// �V�X�e���̏�����
	LoadSystem();

	// �c�@����ݒ�
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_nPlayerStock[nCntPlayer] = m_nPlayerStockIni;
	}

	// �J�����̐���
	CreateCamera();

	// UI�𐶐�
	CreateUI();

	// �v���C���[�̐���
	CreatePlayer();

	// �X�e�[�W�ԍ��̌������v�Z����
	CalcStageDigits();

	// �}�b�v�̐���
	CreateMap();

	// ���͐}�̐���
	CreatePowerMap();

	// �t�@���t�@�[�����Đ�
	CManager::GetSound()->PlaySound(GAME_SE_FANFARE_IDX);

	// ���[�J��1P�v���C�Ȃ�΂�����l�̃v���C���[�������Ă���
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{
		if (m_pPlayer[MAX_NUM_PLAYER - 1] != NULL)
		{
			m_pPlayer[MAX_NUM_PLAYER - 1]->Uninit();
			m_pPlayer[MAX_NUM_PLAYER - 1] = NULL;
		}
	}

	// MVP�ŕK�v�ȏ��
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_nJammer[nCntPlayer] = 0;
		m_nCream[nCntPlayer] = 0;
		m_nCollector[nCntPlayer] = 0;
	}

	return S_OK;
}

//=============================================================================
// �Q�[���̏I������
//=============================================================================
void CGame::Uninit(void)
{
	// �e��|�C���^�̊J��
	ReleaseUI();
	ReleasePlayer();
	ReleasePlayerManager();
	ReleaseEnemyManager();
	ReleaseGameOverLogo();
	ReleaseStageBg();
	ReleaseStageLogo();
	ReleaseStageNumber();
	ReleaseGameResult();
	ReleasePause();
	ReleaseNotPause();
	ReleaseCreamMist();
	ReleasePowerMap();

	// �ǂݍ��ރ}�b�v�̃t�@�C�����̃|�C���^���J��
	ReleaseMapFilePointer();

	// ���ʂ̏I������
	CBasemode::Uninit();

	// �S�ẴI�u�W�F�N�g�J��
	CScene::ReleaseAll();

	if (m_State != STATE_END_RETRY)
	{// ��蒼���̏I����ԂłȂ�
		// �N���C�A���g���J������
		CManager::ReleaseClient();
	}

	// ���U���g�̎擾
	CResult *pResult = CManager::GetResult();

	// MVP�̏������U���g�ɑ��
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		pResult->SetScore(m_nScore[nCntPlayer], nCntPlayer);
		pResult->SetJammer(m_nJammer[nCntPlayer], nCntPlayer);
		pResult->SetCream(m_nCream[nCntPlayer], nCntPlayer);
		pResult->SetCollector(m_nCollector[nCntPlayer], nCntPlayer);
	}

	// �n�C�X�R�A���X�V�������ǂ����`�F�b�N
	int nHighScore = CTitle::GetHighScore();
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_nScore[nCntPlayer] > nHighScore)
		{// �l���X�R�A���n�C�X�R�A��������
			CTitle::SetHighScore(m_nScore[nCntPlayer]);
			CResult::SetHighScore();
		}
	}

	// BGM���~�߂�
	CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX);
	StopBGM();
}

//=============================================================================
// �Q�[���̍X�V����
//=============================================================================
void CGame::Update(void)
{
	// �J�����̍X�V����
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->Update();
	}

	// �G�𓮂����邩�`�F�b�N
	m_nEnemyMoveCounter--;
	if (m_nEnemyMoveCounter <= 0)
	{
		m_nEnemyMoveCounter = 0;
		m_bEnemyMove = true;
	}

	// ��Ԃɂ���ď����킯
	switch (m_State)
	{
	case STATE_STAGE_SELECT:
		StageSelectUpdate();
		CScene::DeathCheck();
		break;
	case STATE_STAGE_DISP:
		StageDispUpdate();
		CScene::DeathCheck();
		break;
	case STATE_NORMAL:
		NormalUpdate();
		CScene::UpdateAll();
		break;
	case STATE_PAUSE:
		PauseUpdate();
		break;
	case STATE_GAMEOVER:
		GameOverUpdate();
		CScene::UpdateAll();
		break;
	case STATE_RESULT:
		ResultUpdate();
		break;
	case STATE_CHANGE_MAP:
		ChangeMapUpdate();
		CScene::UpdateAll();
		break;
	case STATE_PREV_MAP:
		ChangePrevMap();
		CScene::DeathCheck();
		break;
	case STATE_NEXT_MAP:
		ChangeNextMap();
		CScene::DeathCheck();
		break;
	case STATE_END:
		EndUpdate();
		CScene::UpdateAll();
		break;
	case STATE_END_RETRY:
		EndRetryUpdate();
		break;
	case STATE_END_QUIT:
		EndQuitUpdate();
		break;
	}

#ifdef GAME_MAPEVENT
	// �}�b�v�̏�Ԃɂ���ď����킯
	switch (CCharaSelect::GetStageType())
	{
	case MAPTYPE_HINAMATSURI:  // �ЂȍՂ�
		MapEvent_Hinamatsuri();
		break;
	}
#endif

	// �v���C���[�����X�|�[�������邩�`�F�b�N
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{// ���[�J��1P�v���C�Ȃ��
		CheckPlayerResSpawn(0);
	}
	else if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL2P)
	{// ���[�J��2P�v���C�Ȃ��
		for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{
			CheckPlayerResSpawn(nCntPlayer);
		}
	}
	else if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C��2P�v���C�Ȃ��
		CheckPlayerResSpawn(CManager::GetClient()->GetClientId());
	}

	// �G�𐶐����邩�`�F�b�N����
	int nIdxClient = 0;
	if (CManager::GetClient() != NULL)
	{
		nIdxClient = CManager::GetClient()->GetClientId();
	}
	if (nIdxClient == 0 && m_State != STATE_STAGE_SELECT && m_State != STATE_STAGE_DISP && m_State != STATE_PAUSE)
	CheckEnemySpawn(m_nGameCounter);

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		// �T�[�o�[�֑���f�[�^��ݒ肷��
		SetDataToServer();

		// �N���C�A���g�̍X�V����
		CManager::ClientUpdate();

		// �T�[�o�[���瑗��ꂽ�f�[�^��ݒ肷��
		GetDataFromServer();
	}

	// �f�[�^�N���A
	m_bDeletePlayerFlag = false;
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
	m_nNumDeleteItem = 0;
	strcpy(m_aDeleteItem, "\0");
	m_bHitBulletFlag = false;
	m_bHitHeadQuarters = false;

	CDebugProc::Print(1, "�Q�[�����\n");
	if (CManager::GetClient() != NULL)
	{
		CDebugProc::Print(1, "�N���C�A���g�ԍ�      : %d\n", CManager::GetClient()->GetClientId());
	}
	CDebugProc::Print(1, "�v���C���[1�̎�ޔԍ� : %d\n", CCharaSelect::GetPlayerNumber(0));
	CDebugProc::Print(1, "�v���C���[2�̎�ޔԍ� : %d\n\n", CCharaSelect::GetPlayerNumber(1));
	CDebugProc::Print(1, "���݂̃X�e�[�W�ԍ�    : %d\n", m_nStageIdx);
	CDebugProc::Print(1, "�Q�[���J�E���^�[      : %d\n", m_nGameCounter);
	CDebugProc::Print(1, "���ݐ������ꂽ�G�̐�  : %d\n", m_nSpawnEnemyCount);
	CDebugProc::Print(1, "�c��̓G�̐�          : %d\n", m_nNumEnemy);
	CDebugProc::Print(1, "�v���C���[1�̎c�@��   : %d\n", m_nPlayerStock[0]);
	CDebugProc::Print(1, "�v���C���[2�̎c�@��   : %d\n", m_nPlayerStock[1]);
	CDebugProc::Print(1, "���X�|�[���J�E���^�[  : %d\n", m_nPlayerRespawnCounter);
	CDebugProc::Print(1, "�v���C���[1�̒e�̐�   : %d\n", CBulletPlayer::GetNumAll_0());
	CDebugProc::Print(1, "�v���C���[2�̒e�̐�   : %d\n", CBulletPlayer::GetNumAll_1());
	CDebugProc::Print(1, "�G�̒e�̐�            : %d\n", CBulletEnemy::GetNumAll());
	CDebugProc::Print(1, "�u���b�N�̐�          : %d\n", CBlock::GetNumAll());
	CDebugProc::Print(1, "�X�|�[�������G�̐�    : %d\n", CEnemy::GetSpawnCounter());
}

//=============================================================================
// �Q�[���̕`�揈��
//=============================================================================
void CGame::Draw(void)
{
	// �J�����̐ݒ�
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->BindCamera();
	}

	// �V�[���N���X�̕`�揈��
	CScene::DrawAll();
}


//*****************************************************************************
//
// �T�[�o�[�̍X�V�p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̏����u���b�N�̃f�[�^��ݒ�
//=============================================================================
void CGame::DeleteBlock(const int nIdx)
{
	m_nNumDeleteBlock++;
	char aData[64] = "\0";
	sprintf(aData, "%d", nIdx);
	strcat(m_aDeleteBlock, aData);
	strcat(m_aDeleteBlock, " ");
}

//=============================================================================
// �Q�[���̏����G�̃f�[�^��ݒ�
//=============================================================================
void CGame::DeleteEnemy(const int nIdx)
{
	m_nNumDeleteEnemy++;
	char aData[64] = "\0";
	sprintf(aData, "%d", nIdx);
	strcat(m_aDeleteEnemy, aData);
	strcat(m_aDeleteEnemy, " ");
}

//=============================================================================
// �Q�[���̏����A�C�e���̃f�[�^��ݒ�
//=============================================================================
void CGame::DeleteItem(const int nIdx)
{
	m_nNumDeleteItem++;
	char aData[64] = "\0";
	sprintf(aData, "%d", nIdx);
	strcat(m_aDeleteItem, aData);
	strcat(m_aDeleteItem, " ");
}

//=============================================================================
// �Q�[���̃v���C���[����ɂ��鏈��
//=============================================================================
void CGame::DeletePlayer(CPlayer *pPlayer, const int nIdx)
{
	SetPlayer(pPlayer, nIdx);
	if (m_nPlayerStock[nIdx] <= 0)
	{
		SetState(STATE_GAMEOVER);
	}

	// �z�X�g����Ȃ�������
	if (CManager::GetClient() == NULL)return;
	if (CManager::GetClient()->GetClientId() == 0 && nIdx != 0)
	{
		m_bDeletePlayerFlag = true;
	}
}

//=============================================================================
// �Q�[���̑���v���C���[�̒e�������������ǂ����ݒ肷��
//=============================================================================
void CGame::HitBullet(void)
{
	m_bHitBulletFlag = true;
}

//=============================================================================
// �Q�[���̎i�ߕ��ɒe�������������ǂ����ݒ肷��
//=============================================================================
void CGame::HitHeadQuarters(void)
{
	m_bHitHeadQuarters = true;
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServer(void)
{
	if (CManager::GetClient() == NULL || CManager::GetClient()->GetConnected() == false)return;

	// �v���C���[�̃f�[�^��ݒ�
	SetDataToServerFromPlayer();

	if (CManager::GetClient()->GetClientId() == 0)
	{// �������z�X�g�Ȃ�
	    // �Q�[���̃f�[�^��ݒ�
		SetDataToServerFromGame();

		// �G�̃f�[�^��ݒ�
		SetDataToServerFromEnemy();

		// �G�̒e�̃f�[�^��ݒ�
		SetDataToServerFromEnemyBullet();

		// �����u���b�N�̃f�[�^��ݒ�
		SetDataToServerFromDeleteBlock();

		// �A�C�e���̃f�[�^��ݒ�
		SetDataToServerFromItem();

		// �G�̃X�|�[������ݒ�
		SetDataToServerFromSpawnEnemyCount();

		// �}�b�v�C�x���g�̃f�[�^��ݒ�
		SetDataToServerFromMapEvent();
	}

	// �v���C���[�̒e����ݒ�
	SetDataToServerFromPlayerBullet();

	if (CManager::GetClient()->GetClientId() != 0)
	{// �������z�X�g�łȂ��Ȃ�
		// �����u���b�N�̃f�[�^��ݒ�
		SetDataToServerFromDeleteBlock();

		// �����G�̃f�[�^��ݒ�
		SetDataToServerFromDeleteEnemy();

		// ���g�̏�Ԃ�ݒ�
		SetDataToServerFromClientState();

		// �����A�C�e���̃f�[�^��ݒ�
		SetDataToServerFromDeleteItem();

		// �i�ߕ��j��̃f�[�^��ݒ�
		SetDataToServerFromHitHeadQuarters();
	}

	// �|�����G�̐���ݒ�
	SetDataToServerFromBreakEnemy();

	// �e�ɓ����������ǂ����ݒ�
	SetDataToServerFromHitBullet();
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���v���C���[�f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromPlayer(void)
{
	// �v���C���[�̐l����ݒ�
	if (m_pPlayer[CManager::GetClient()->GetClientId()] == NULL)
	{
		// �v���C���[�����Ȃ����Ƃ�ݒ�
		CManager::GetClient()->Print("0");
		CManager::GetClient()->Print(" ");

		// �v���C���[�̎c�@����ݒ�
		CManager::GetClient()->Print("%d", m_nPlayerStock[CManager::GetClient()->GetClientId()]);
		CManager::GetClient()->Print(" ");

		if (CManager::GetClient()->GetClientId() == 0)
		{
			CManager::GetClient()->Print("%d", (int)m_bDeletePlayerFlag);
			CManager::GetClient()->Print(" ");
		}
		return;
	}
	else if(m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
	{
		// �v���C���[�����邱�Ƃ�ݒ�
		CManager::GetClient()->Print("1");
		CManager::GetClient()->Print(" ");

		// �v���C���[�̎c�@����ݒ�
		CManager::GetClient()->Print("%d", m_nPlayerStock[CManager::GetClient()->GetClientId()]);
		CManager::GetClient()->Print(" ");

		// �v���C���[�̍��W��ݒ�
		D3DXVECTOR3 PlayerPos = m_pPlayer[CManager::GetClient()->GetClientId()]->GetPos();
		CManager::GetClient()->Print("%.1f %.1f %.1f", PlayerPos.x, PlayerPos.y, PlayerPos.z);
		CManager::GetClient()->Print(" ");

		// �v���C���[�̌�����ݒ�
		D3DXVECTOR3 PlayerRot = m_pPlayer[CManager::GetClient()->GetClientId()]->GetRot();
		CManager::GetClient()->Print("%.1f %.1f %.1f", PlayerRot.x, PlayerRot.y, PlayerRot.z);
		CManager::GetClient()->Print(" ");

		// �v���C���[�̏�Ԃ�ݒ�
		int nPlayerState = m_pPlayer[CManager::GetClient()->GetClientId()]->GetState();
		CManager::GetClient()->Print("%d", nPlayerState);
		CManager::GetClient()->Print(" ");

		// �v���C���[�����G��Ԃ��ǂ�����ݒ�
		int nPlayerHelmet = m_pPlayer[CManager::GetClient()->GetClientId()]->GetHelmet();
		CManager::GetClient()->Print("%d", nPlayerHelmet);
		CManager::GetClient()->Print(" ");
	}

	// �z�X�g��������
	if (CManager::GetClient()->GetClientId() == 0)
	{
		CManager::GetClient()->Print("%d", (int)m_bDeletePlayerFlag);
		CManager::GetClient()->Print(" ");
	}
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���Q�[���f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromGame(void)
{
	// ��Ԃ�ݒ�
	CManager::GetClient()->Print("%d", m_State);
	CManager::GetClient()->Print(" ");

	// �}�b�v�̔ԍ���ݒ�
	CManager::GetClient()->Print("%d", m_nMapIdx);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���G�f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromEnemy(void)
{
	int nNumEnemy = 0;
	char aData[64] = "\0";
	char aEnemyData[MAX_SERVER_DATA] = "\0";

	// �G�̃f�[�^��ݒ�
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// �G�N���X������
				strcat(aEnemyData, GetDataToEnemy((CEnemy*)pScene, aData));
				strcpy(aData, "\0");
				nNumEnemy++;
			}
			pScene = pSceneNext;
		}
	}

	// �G�̐���ݒ�
	CManager::GetClient()->Print("%d", nNumEnemy);
	CManager::GetClient()->Print(" ");

	// �G�̃f�[�^��ݒ�
	CManager::GetClient()->Print("%s", aEnemyData);
}

//=============================================================================
// �G�̃f�[�^�𕶎���ɕϊ����鏈��
//=============================================================================
char *CGame::GetDataToEnemy(CEnemy *pEnemy, char *pStr)
{
	char aData[64] = "\0";

	// �G�̍��W��ݒ�
	D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
	sprintf(aData, "%.1f %.1f %.1f", EnemyPos.x, EnemyPos.y, EnemyPos.z);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// �G�̌�����ݒ�
	D3DXVECTOR3 EnemyRot = pEnemy->GetRot();
	sprintf(aData, "%.1f %.1f %.1f", EnemyRot.x, EnemyRot.y, EnemyRot.z);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// �G�̎��
	int nEnemyType = pEnemy->GetType();
	sprintf(aData, "%d", nEnemyType);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// �G�̔ԍ�
	int nEnemyIdx = pEnemy->GetIdx();
	sprintf(aData, "%d", nEnemyIdx);
	strcat(pStr, aData);
	strcat(pStr, " ");

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���v���C���[�̒e�f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromPlayerBullet(void)
{
	int nNumBullet = 0;
	char aData[64] = "\0";
	char aBulletData[MAX_SERVER_DATA] = "\0";

	// ���݂̃v���C���[�̒e�̐���ݒ�
	if (CManager::GetClient()->GetClientId() == 0)
	{
		nNumBullet = CBulletPlayer::GetNumAll_0();
	}
	else
	{
		nNumBullet = CBulletPlayer::GetNumAll_1();
	}

	// �e�̐���ݒ�
	CManager::GetClient()->Print("%d", nNumBullet);
	CManager::GetClient()->Print(" ");
	if (nNumBullet < 1) return;

	// �e�̃f�[�^��ݒ肷��
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET)
			{// �e�N���X������
				strcat(aBulletData, GetDataToPlayerBullet((CBullet*)pScene, aData));
				strcpy(aData, "\0");
			}
			pScene = pSceneNext;
		}
	}

	// �e�̃f�[�^��ݒ�
	CManager::GetClient()->Print("%s", aBulletData);
}

//=============================================================================
// �v���C���[�̒e�̃f�[�^�𕶎���ɕϊ����鏈��
//=============================================================================
char *CGame::GetDataToPlayerBullet(CBullet *pBullet, char *pStr)
{
	if (pBullet->GetType() == CManager::GetClient()->GetClientId())
	{// �����̃v���C���[���������e�̏�����������
		char aData[64] = "\0";

		// �e�̍��W��ݒ�
		D3DXVECTOR3 BulletPos = pBullet->GetPos();
		sprintf(aData, "%.1f %.1f %.1f", BulletPos.x, BulletPos.y, BulletPos.z);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// �e�̔ԍ���ݒ�
		int nBulletIdx = pBullet->GetIdx();
		sprintf(aData, "%d", nBulletIdx);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// �e�̎�ނ�ݒ�
		int nBulletType = pBullet->GetType();
		sprintf(aData, "%d", nBulletType);
		strcat(pStr, aData);
		strcat(pStr, " ");
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���G�̒e�f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromEnemyBullet(void)
{
	int nNumBullet = 0;
	char aData[64] = "\0";
	char aBulletData[MAX_SERVER_DATA] = "\0";

	// �G�̒e�̐���ݒ�
	nNumBullet = CBulletEnemy::GetNumAll();

	// �e�̐���ݒ�
	CManager::GetClient()->Print("%d", nNumBullet);
	CManager::GetClient()->Print(" ");
	if (nNumBullet < 1) return;

	// �e�̃f�[�^��ݒ肷��
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET)
			{// �e�N���X������
				strcat(aBulletData, GetDataToEnemyBullet((CBullet*)pScene, aData));
				strcpy(aData, "\0");
			}
			pScene = pSceneNext;
		}
	}

	// �e�̃f�[�^��ݒ�
	CManager::GetClient()->Print("%s", aBulletData);
}

//=============================================================================
// �G�̒e�̃f�[�^�𕶎���ɕϊ����鏈��
//=============================================================================
char *CGame::GetDataToEnemyBullet(CBullet *pBullet, char *pStr)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY)
	{// �G�̒e�f�[�^��ݒ肷��
		char aData[64] = "\0";

		// �e�̍��W��ݒ�
		D3DXVECTOR3 BulletPos = pBullet->GetPos();
		sprintf(aData, "%.1f %.1f %.1f", BulletPos.x, BulletPos.y, BulletPos.z);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// �e�̔ԍ���ݒ�
		int nBulletIdx = pBullet->GetIdx();
		sprintf(aData, "%d", nBulletIdx);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// �e�̎�ނ�ݒ�
		int nBulletType = pBullet->GetType();
		sprintf(aData, "%d", nBulletType);
		strcat(pStr, aData);
		strcat(pStr, " ");
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���A�C�e���f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromItem(void)
{
	int nNumItem = 0;
	char aData[64] = "\0";
	char aItemData[MAX_SERVER_DATA] = "\0";

	// �A�C�e���̐���ݒ�
	nNumItem = CItem::GetNumAll();

	// �A�C�e���̐���ݒ�
	CManager::GetClient()->Print("%d", nNumItem);
	CManager::GetClient()->Print(" ");
	if (nNumItem < 1) return;

	// �A�C�e���̃f�[�^��ݒ肷��
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ITEM)
			{// �A�C�e���N���X������
				strcat(aItemData, GetDataToItem((CItem*)pScene, aData));
				strcpy(aData, "\0");
			}
			pScene = pSceneNext;
		}
	}

	// �A�C�e���̃f�[�^��ݒ�
	CManager::GetClient()->Print("%s", aItemData);
}

//=============================================================================
// �Q�[���̃A�C�e���̃f�[�^�𕶎���ɕϊ����鏈��
//=============================================================================
char *CGame::GetDataToItem(CItem *pItem, char *pStr)
{
	char aData[64] = "\0";

	// �A�C�e���̍��W��ݒ�
	D3DXVECTOR3 ItemPos = pItem->GetPos();
	sprintf(aData, "%.1f %.1f %.1f", ItemPos.x, ItemPos.y, ItemPos.z);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// �A�C�e���̔ԍ���ݒ�
	int nItemIdx = pItem->GetIdx();
	sprintf(aData, "%d", nItemIdx);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// �A�C�e���̎�ނ�ݒ�
	int nItemType = pItem->GetType();
	sprintf(aData, "%d", nItemType);
	strcat(pStr, aData);
	strcat(pStr, " ");

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ�������u���b�N�̃f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromDeleteBlock(void)
{
	CManager::GetClient()->Print("%d", m_nNumDeleteBlock);
	CManager::GetClient()->Print(" ");
	if (m_nNumDeleteBlock > 0)
	{
		CManager::GetClient()->Print("%s", m_aDeleteBlock);
		CManager::GetClient()->Print(" ");
	}
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ�������G�̃f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromDeleteEnemy(void)
{
	CManager::GetClient()->Print("%d", m_nNumDeleteEnemy);
	CManager::GetClient()->Print(" ");
	if (m_nNumDeleteEnemy > 0)
	{
		CManager::GetClient()->Print("%s", m_aDeleteEnemy);
		CManager::GetClient()->Print(" ");
	}
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���G�̐���ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromSpawnEnemyCount(void)
{
	CManager::GetClient()->Print("%d", m_nSpawnEnemyCount);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ����Ԃ�ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromClientState(void)
{
	// ��Ԃ�ݒ�
	CManager::GetClient()->Print("%d", m_State);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɏ����A�C�e���̃f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromDeleteItem(void)
{
	CManager::GetClient()->Print("%d", m_nNumDeleteItem);
	CManager::GetClient()->Print(" ");
	if (m_nNumDeleteItem > 0)
	{
		CManager::GetClient()->Print("%s", m_aDeleteItem);
		CManager::GetClient()->Print(" ");
	}
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���|�����G�̐���ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromBreakEnemy(void)
{
	// �ԍ���ݒ�
	int nIdxClient = 0;
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		nIdxClient = pClient->GetClientId();
	}

	// �G�̓|��������ݒ�
	for (int nCntType = 0; nCntType < CEnemy::TYPE_MAX; nCntType++)
	{
		CManager::GetClient()->Print("%d", m_nNumBreakEnemy[nIdxClient][nCntType]);
		CManager::GetClient()->Print(" ");
	}

	// �X�R�A��ݒ�
	CManager::GetClient()->Print("%d", m_nScore[nIdxClient]);
	CManager::GetClient()->Print(" ");

	// �ז������񐔂�ݒ�
	CManager::GetClient()->Print("%d", m_nJammer[nIdxClient]);
	CManager::GetClient()->Print(" ");

	// �N���[���ɂɓ��������񐔂�ݒ�
	CManager::GetClient()->Print("%d", m_nCream[nIdxClient]);
	CManager::GetClient()->Print(" ");

	// �A�C�e�����擾�����񐔂�ݒ�
	CManager::GetClient()->Print("%d", m_nCollector[nIdxClient]);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���|�����G�̐���ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromMapEvent(void)
{
	int nHinaEvent = m_HinaEvent;
	CManager::GetClient()->Print("%d", nHinaEvent);
	CManager::GetClient()->Print(" ");
	CManager::GetClient()->Print("%d", m_nNextEvent);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���e�ɓ����������ǂ�����ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromHitBullet(void)
{
	CManager::GetClient()->Print("%d", (int)m_bHitBulletFlag);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���i�ߕ��ɒe�������������ǂ�����ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromHitHeadQuarters(void)
{
	CManager::GetClient()->Print("%d", (int)m_bHitHeadQuarters);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::GetDataFromServer(void)
{
	// ����ꂽ���b�Z�[�W���擾
	if (CManager::GetClient() == NULL)return;
	char *pStr = CManager::GetClient()->GetReceiveMessage();
	if (pStr == NULL || CManager::GetClient()->GetConnected() == false || *pStr == *"???")
	{// ���b�Z�[�W��M�Ɏ��s || �T�[�o�[�ɐڑ��ł��Ă��Ȃ�
		if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL && m_State == STATE_NORMAL)
		{// �v���C���[�������Ă���
			m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->Uninit();
			m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] = NULL;
		}
		return;
	}

	// �v���C���[�̃f�[�^��ݒ�
	pStr = SetDataToPlayerFromServer(pStr);

	if (CManager::GetClient()->GetClientId() != 0)
	{// �z�X�g����Ȃ�������
	    // �Q�[���̏�Ԃ�ݒ�
		pStr = SetDataToGameFromServer(pStr);

		// �G�̃f�[�^��ݒ�
		pStr = SetDataToEnemyFromServer(pStr);

		// �G�̒e�̃f�[�^��ݒ�
		pStr = SetDataToEnemyBulletFromServer(pStr);

		// �����u���b�N�̃f�[�^��ݒ�
		pStr = SetDataToDeleteBlock(pStr);

		// �A�C�e���̃f�[�^��ݒ�
		pStr = SetDataToItemFromServer(pStr);

		// �G�̐��̃f�[�^��ݒ�
		pStr = SetDataToSpawnEnemyCount(pStr);

		// �}�b�v�C�x���g�̃f�[�^��ݒ�
		pStr = SetDataToMapEvent(pStr);
	}

	// �v���C���[�̒e�̃f�[�^��ݒ�
	pStr = SetDataToPlayerBulletFromServer(pStr);

	if (CManager::GetClient()->GetClientId() == 0)
	{// �z�X�g��������
		// �����u���b�N�̃f�[�^��ݒ�
		pStr = SetDataToDeleteBlock(pStr);

		// �����G�̃f�[�^��ݒ�
		pStr = SetDataToDeleteEnemy(pStr);

		// ����̏�Ԃ�ݒ�
		pStr = SetDataToClientState(pStr);

		// �����A�C�e���̃f�[�^��ݒ�
		pStr = SetDataToDeleteItem(pStr);

		// �i�ߕ��ɒe�������������ǂ����ݒ�
		pStr = SetDataToHitHeadQuarters(pStr);
	}

	// �G��|��������ݒ�
	pStr = SetDataToBreakEnemy(pStr);

	// �e���q�b�g�������ǂ����ݒ�
	pStr = SetDataToHitBullet(pStr);

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck();
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^���v���C���[�ɐݒ肷�鏈��
//=============================================================================
char *CGame::SetDataToPlayerFromServer(char *pStr)
{
	int nWord = 0;
	int nNumPlayer = 0;
	int nNumStock = 0;
	int nNumber = (CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER;

	// �v���C���[�̐l����ǂݎ��
	nNumPlayer = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �v���C���[�̎c�@����ǂݎ��
	nNumStock = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	if (m_nPlayerStock[nNumber] != nNumStock)
	{// �c�@�����ς���Ă���
		m_nPlayerStock[nNumber] = nNumStock;
		if (m_pUI != NULL)
		{
			m_pUI->ReCreatePlayerStock(&m_nPlayerStock[0]);
		}
	}

	if (nNumPlayer == 0)
	{// �v���C���[�����݂��Ȃ�
		if (m_pPlayer[nNumber] != NULL && m_State == STATE_NORMAL)
		{// �v���C���[�������Ă���
			m_pPlayer[nNumber]->SetDeathEffect();
			m_pPlayer[nNumber]->Uninit();
			m_pPlayer[nNumber] = NULL;
			if (m_pUI != NULL)
			{
				m_pUI->ReCreatePlayerStock(&m_nPlayerStock[0]);
			}
		}
	}
	else if(nNumPlayer == 1)
	{// �v���C���[�����݂���
		CMap *pMap = GetMap();
		bool bPlayerSpawn = false;
		if (m_pPlayer[nNumber] == NULL && pMap != NULL)
		{// ���������m�ۂ���Ă��Ȃ�
			// �v���C���[�̍��W��ǂݎ��
			float fGetPlayerPosX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�̌�����ǂݎ��
			float fGetPlayerRotX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�̏�Ԃ�ǂݎ��
			int nGetPlayerState = CFunctionLib::ReadInt(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�����G��Ԃ��ǂ�����ǂݎ��
			bool bPlayerHelmet = CFunctionLib::ReadBool(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			m_pPlayer[nNumber] = m_pPlayerManager[nNumber]->SetPlayer(D3DXVECTOR3(fGetPlayerPosX, fGetPlayerPosY, fGetPlayerPosZ), D3DXVECTOR3(fGetPlayerRotX, fGetPlayerRotY, fGetPlayerRotZ), nNumber);
			bPlayerSpawn = true;
		}
		else if (m_pPlayer[nNumber] != NULL)
		{// ���������m�ۂł��Ă���
			// �v���C���[�̍��W��ǂݎ��
			float fGetPlayerPosX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�̌�����ǂݎ��
			float fGetPlayerRotX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�̏�Ԃ�ǂݎ��
			int nGetPlayerState = CFunctionLib::ReadInt(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�����G��Ԃ��ǂ�����ǂݎ��
			bool bPlayerHelmet = CFunctionLib::ReadBool(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�ɒl��ݒ�
			if (m_pPlayer[nNumber] != NULL)
			{
				m_pPlayer[nNumber]->SetPos(D3DXVECTOR3(fGetPlayerPosX, fGetPlayerPosY, fGetPlayerPosZ));
				m_pPlayer[nNumber]->SetRot(D3DXVECTOR3(fGetPlayerRotX, fGetPlayerRotY, fGetPlayerRotZ));
				if (m_pPlayer[nNumber]->GetState() != nGetPlayerState)
				{
					m_pPlayer[nNumber]->SetState((CPlayer::STATE)nGetPlayerState);
				}
				m_pPlayer[nNumber]->SetHelmet(bPlayerHelmet);
			}

			// �v���C���[���������ꂽ�΂�����Ȃ�G�t�F�N�g���o��
			CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
			if (pEffectManager != NULL && bPlayerSpawn == true)
			{
				D3DXVECTOR3 EffectPos = D3DXVECTOR3(fGetPlayerPosX, fGetPlayerPosY, fGetPlayerPosZ);
				pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, GAME_PLAYER_SPAWN_EFFECT_IDX);
				pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, GAME_PLAYER_SPAWN_EFFECT_IDX + 1);
			}
		}
	}


	// �z�X�g����Ȃ�������
	if (CManager::GetClient()->GetClientId() != 0)
	{
		bool bDeletePlayerFlag = CFunctionLib::ReadBool(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		if (bDeletePlayerFlag == true && m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
		{
			m_pPlayer[CManager::GetClient()->GetClientId()]->SetDeathEffect();
			m_pPlayer[CManager::GetClient()->GetClientId()]->Uninit();
			m_pPlayer[CManager::GetClient()->GetClientId()] = NULL;
		}
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^���Q�[���ɐݒ肷�鏈��
//=============================================================================
char *CGame::SetDataToGameFromServer(char *pStr)
{
	int nGameState = CFunctionLib::ReadInt(pStr, "");
	int nWord = 0;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	int nMapIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// ��Ԃ�ݒ�
	m_StateOld = m_State;
	m_State = (STATE)nGameState;
	if (m_State != STATE_END && m_State != STATE_GAMEOVER && m_State != STATE_RESULT)
	{// �I����Ԃł��Q�[���I�[�o�[��Ԃł��Q�[�������U���g��Ԃł��Ȃ�
		m_nStateCounter = 0;
	}

	// �}�b�v�̔ԍ���ݒ�
	m_nMapIdx = nMapIdx;

	// ��Ԃɂ���ď����킯
	if (m_State == STATE_STAGE_DISP && GetMap() == NULL)
	{
		CreateMap();
		CManager::GetSound()->PlaySound(GAME_SE_FANFARE_IDX);
	}
	else if (m_State == STATE_STAGE_SELECT && GetMap() != NULL)
	{
		ReleaseMap();
		DeleteGameObject();
	}
	else if (m_State == STATE_STAGE_DISP && m_StateOld == STATE_NORMAL)
	{
		ReleaseMap();
		DeleteGameObject();
	}
	else if (m_State == STATE_NORMAL && m_StateOld == STATE_STAGE_DISP)
	{
		ReleaseStageBg();
		ReleaseStageLogo();
		ReleaseStageNumber();
		SetPlayerPosToSpawn();
		CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX);
	}
	else if (m_State == STATE_STAGE_DISP || m_State == STATE_STAGE_SELECT)
	{
		ReleaseStageNumber();
		CalcStageDigits();
		CreateStageNumber();
	}
	else if (m_State == STATE_RESULT && m_StateOld == STATE_END)
	{
		CreateGameResult();
		CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX);
	}
	else if (m_State == STATE_PAUSE && m_StateOld == STATE_NORMAL)
	{
		if (m_bPauseOpen == true)
		{
			m_bPauseOpen = false;
			m_State = STATE_NORMAL;
			return pStr;
		}
		m_bPauseOpen = true;
		CreateNotPause();
		CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX, true);
	}
	else if (m_State == STATE_NORMAL && m_StateOld == STATE_PAUSE)
	{
		if (m_pPause != NULL)
		{
			m_State = STATE_PAUSE;
			return pStr;
		}

		m_bPauseOpen = false;
		ReleaseNotPause();
		CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX, true);
	}
	else if (m_State == STATE_END_RETRY && m_StateOld == STATE_PAUSE
		|| m_State == STATE_END_QUIT && m_StateOld == STATE_PAUSE)
	{
		ReleaseNotPause();
	}

	// ��蒼���o�O���p
	if (m_StateOld == STATE_END_RETRY && m_State != STATE_END_RETRY)
	{
		m_State = STATE_END_RETRY;
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^��G�ɐݒ肷�鏈��
//=============================================================================
char *CGame::SetDataToEnemyFromServer(char *pStr)
{
	int nWord = 0;
	int nNumEnemy = 0;

	// ��������Ă���G�̐���ǂݎ��
	nNumEnemy = CFunctionLib::ReadInt(pStr, "");
	m_nNumEnemy = nNumEnemy;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �����킹
	ReleaseEnemy(nNumEnemy);
	if (nNumEnemy == 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < (ENEMY_PRIORITY + 1); nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY && nNumEnemy > 0)
			{// �G�N���X������
				pStr = SetDataToEnemy((CEnemy*)pScene, pStr);
				nNumEnemy--;
			}
			pScene = pSceneNext;
		}
	}

	// ����Ȃ��G�͐V���ɐ������f�[�^��ݒ�
	for (int nCntEnemy = 0; nCntEnemy < nNumEnemy; nCntEnemy++)
	{
		pStr = SetDataToCreateEnemy(pStr);
	}
	CEnemy::SetNumAll(m_nNumEnemy);

	return pStr;
}

//=============================================================================
// ��������f�[�^�ɕϊ����G�ɐݒ肷��
//=============================================================================
char *CGame::SetDataToEnemy(CEnemy *pEnemy, char *pStr)
{
	int nWord = 0;
	float fEnemyPosX = 0.0f;
	float fEnemyPosY = 0.0f;
	float fEnemyPosZ = 0.0f;
	float fEnemyRotX = 0.0f;
	float fEnemyRotY = 0.0f;
	float fEnemyRotZ = 0.0f;
	int nEnemyType = 0;
	int nEnemyIdx = 0;

	// �G�̍��W��ǂݎ��
	fEnemyPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̌�����ǂݎ��
	fEnemyRotX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̎�ޔԍ�
	nEnemyType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̔ԍ�
	nEnemyIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̎�ނ��擾���Ⴄ�Ȃ���ւ���
	int nEnemyTypeOld = pEnemy->GetType();
	if (nEnemyType != nEnemyTypeOld)
	{
		// �j������
		pEnemy->Uninit();
		pEnemy = NULL;
		CScene::DeathCheck(ENEMY_PRIORITY);

		// �������Ȃ���
		CEnemy_ListData *pEnemyData = GetMap()->GetEnemyListData(nEnemyIdx);
		D3DXVECTOR3 pos = D3DXVECTOR3(fEnemyPosX, fEnemyPosY, fEnemyPosZ);
		D3DXVECTOR3 rot = D3DXVECTOR3(fEnemyRotX, fEnemyRotY, fEnemyRotZ);
		pEnemy = m_pEnemyManager[nEnemyType]->SetEnemy(pos, rot, nEnemyType);
		if (pEnemy == NULL && pEnemyData != NULL)return pStr;
		if (pEnemyData->GetItem() == true)
		{
			pEnemy->SetItemData(pEnemyData->GetItemType());
		}
		pEnemy->SetAIData(pEnemyData->GetAI_BulletEva(), pEnemyData->GetAI_MassEva(), pEnemyData->GetAI_DownEva());
	}
	else
	{// ������ނ̓G�ł���
		// �G�Ƀf�[�^��ݒ�
		pEnemy->SetPos(D3DXVECTOR3(fEnemyPosX, fEnemyPosY, fEnemyPosZ));
		pEnemy->SetRot(D3DXVECTOR3(fEnemyRotX, fEnemyRotY, fEnemyRotZ));
		pEnemy->SetIdx(nEnemyIdx);
	}

	return pStr;
}

//=============================================================================
// ��������f�[�^�ɕϊ������������G�ɐݒ肷��
//=============================================================================
char *CGame::SetDataToCreateEnemy(char *pStr)
{
	int nWord = 0;
	float fEnemyPosX = 0.0f;
	float fEnemyPosY = 0.0f;
	float fEnemyPosZ = 0.0f;
	float fEnemyRotX = 0.0f;
	float fEnemyRotY = 0.0f;
	float fEnemyRotZ = 0.0f;
	int nEnemyType = 0;
	int nEnemyIdx = 0;

	// �G�̍��W��ǂݎ��
	fEnemyPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̌�����ǂݎ��
	fEnemyRotX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̎�ޔԍ�
	nEnemyType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̔ԍ�
	nEnemyIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�𐶐�����(�����ɕK�v�ȃf�[�^��ݒ�)
	CEnemy_ListData *pEnemyData = GetMap()->GetEnemyListData(nEnemyIdx);
	D3DXVECTOR3 pos = D3DXVECTOR3(fEnemyPosX, fEnemyPosY, fEnemyPosZ);
	D3DXVECTOR3 rot = D3DXVECTOR3(fEnemyRotX, fEnemyRotY, fEnemyRotZ);
	CEnemy *pEnemy = m_pEnemyManager[nEnemyType]->SetEnemy(pos, rot, nEnemyType);

	if (pEnemy == NULL)return pStr;
	// �G�̃f�[�^��ݒ肷��
	if (pEnemyData->GetItem() == true)
	{
		pEnemy->SetItemData(pEnemyData->GetItemType());
	}
	pEnemy->SetAIData(pEnemyData->GetAI_BulletEva(), pEnemyData->GetAI_MassEva(), pEnemyData->GetAI_DownEva());
	pEnemy->SetIdx(nEnemyIdx);

	return pStr;
}

//=============================================================================
// �v���C���[�̒e�̐������킹�鏈��
//=============================================================================
void CGame::ReleaseEnemy(int nNumEnemy)
{
	// ���݂̓G�̐���ݒ�
	int nAllEnemy = 0;
	nAllEnemy = CEnemy::GetNumAll();
	if (nAllEnemy < nNumEnemy)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < (ENEMY_PRIORITY + 1); nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY && nAllEnemy > nNumEnemy)
			{// �e�N���X������
				ReleaseCheckEnemy((CEnemy*)pScene, &nAllEnemy);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �G��j�����邩�`�F�b�N���鏈��
//=============================================================================
void CGame::ReleaseCheckEnemy(CEnemy *pEnemy, int *pNumEnemy)
{
	pEnemy->SetDeathEffect();
	pEnemy->Uninit();
	pEnemy = NULL;
	*pNumEnemy = *pNumEnemy - 1;
	if (m_pPowerMap != NULL)
	{
		m_pPowerMap->AddGauge();
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck(ENEMY_PRIORITY);
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^���v���C���[�̒e�ɐݒ肷�鏈��
//=============================================================================
char *CGame::SetDataToPlayerBulletFromServer(char *pStr)
{
	int nWord = 0;
	int nNumBullet = 0;

	// ��������Ă���e�̐���ǂݎ��
	nNumBullet = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �����킹
	ReleasePlayerBullet(nNumBullet);
	if (nNumBullet == 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < BULLET_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET)
			{// �e�N���X������
				pStr = SetDataToPlayerBullet((CBullet*)pScene, pStr, &nNumBullet);
			}
			pScene = pSceneNext;
		}
	}

	// ����Ȃ��e�͐V���ɐ������f�[�^��ݒ�
	for (int nCntBullet = 0; nCntBullet < nNumBullet; nCntBullet++)
	{
		pStr = SetDataToCreatePlayerBullet(pStr);
	}

	return pStr;
}

//=============================================================================
// �v���C���[�̒e�̐������킹�鏈��
//=============================================================================
void CGame::ReleasePlayerBullet(int nNumBullet)
{
	// ���݂̃v���C���[�̒e�̐���ݒ�
	int nAllBullet = 0;
	if (CManager::GetClient()->GetClientId() == 0)
	{
		nAllBullet = CBulletPlayer::GetNumAll_1();
	}
	else
	{
		nAllBullet = CBulletPlayer::GetNumAll_0();
	}
	if (nAllBullet < nNumBullet)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < BULLET_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET && nAllBullet > nNumBullet)
			{// �e�N���X������
				ReleaseCheckPlayerBullet((CBullet*)pScene, &nAllBullet);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck(BULLET_PRIORITY - 1);
}

//=============================================================================
// �v���C���[�̒e��j�����邩�`�F�b�N���鏈��
//=============================================================================
void CGame::ReleaseCheckPlayerBullet(CBullet *pBullet, int *pNumBullet)
{
	if (pBullet->GetType() != CBullet::TYPE_ENEMY &&
		pBullet->GetType() != CManager::GetClient()->GetClientId())
	{
		pBullet->Uninit();
		pBullet = NULL;
		*pNumBullet = *pNumBullet - 1;
	}
}

//=============================================================================
// ��������f�[�^�ɕϊ����v���C���[�̒e�ɐݒ肷��
//=============================================================================
char *CGame::SetDataToPlayerBullet(CBullet *pBullet, char *pStr, int *pNumBullet)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY ||
		pBullet->GetType() == CManager::GetClient()->GetClientId()) return pStr;

	int nWord = 0;
	float fBulletPosX = 0.0f;
	float fBulletPosY = 0.0f;
	float fBulletPosZ = 0.0f;
	int nBulletIdx = 0;
	int nBulletType = 0;

	// �e�̍��W��ǂݎ��
	fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̔ԍ���ǂݎ��
	nBulletIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̎�ނ�ǂݎ��
	nBulletType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �v���C���[�̒e�Ƀf�[�^��ݒ�
	pBullet->SetPos(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ));
	pBullet->SetIdx(nBulletIdx);
	pBullet->SetType((CBullet::TYPE)nBulletType);
	*pNumBullet = *pNumBullet - 1;

	return pStr;
}

//=============================================================================
// ��������f�[�^�ɕϊ������������v���C���[�̒e�ɐݒ肷��
//=============================================================================
char *CGame::SetDataToCreatePlayerBullet(char *pStr)
{
	int nWord = 0;
	float fBulletPosX = 0.0f;
	float fBulletPosY = 0.0f;
	float fBulletPosZ = 0.0f;
	int nBulletIdx = 0;
	int nBulletType = 0;

	// �e�̍��W��ǂݎ��
	fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̔ԍ���ǂݎ��
	nBulletIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̎�ނ�ǂݎ��
	nBulletType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�𐶐�����(�����ɕK�v�ȃf�[�^��ݒ�)
	CBulletPlayer *pBullet = CBulletPlayer::Create(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ), INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3,
		(CBullet::TYPE)((CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER), m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]);
	pBullet->SetIdx(nBulletIdx);
	pBullet->SetType((CBullet::TYPE)nBulletType);

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^��G�̒e�ɐݒ肷�鏈��
//=============================================================================
char *CGame::SetDataToEnemyBulletFromServer(char *pStr)
{
	int nWord = 0;
	int nNumBullet = 0;

	// ��������Ă���e�̐���ǂݎ��
	nNumBullet = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �����킹
	ReleaseEnemyBullet(nNumBullet);
	if (nNumBullet == 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < BULLET_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET && nNumBullet > 0)
			{// �e�N���X������
				pStr = SetDataToEnemyBullet((CBullet*)pScene, pStr, &nNumBullet);
			}
			pScene = pSceneNext;
		}
	}

	// ����Ȃ��e�͐V���ɐ������f�[�^��ݒ�
	for (int nCntBullet = 0; nCntBullet < nNumBullet; nCntBullet++)
	{
		pStr = SetDataToCreateEnemyBullet(pStr);
	}

	return pStr;
}

//=============================================================================
// �G�̒e�̐������킹�鏈��
//=============================================================================
void CGame::ReleaseEnemyBullet(int nNumBullet)
{
	// ���݂̓G�̒e�̐���ݒ�
	int nAllBullet = 0;
	nAllBullet = CBulletEnemy::GetNumAll();
	if (nAllBullet < nNumBullet)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < BULLET_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET && nAllBullet > nNumBullet)
			{// �e�N���X������
				ReleaseCheckEnemyBullet((CBullet*)pScene, &nAllBullet);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck(BULLET_PRIORITY - 1);
}

//=============================================================================
// �G�̒e��j�����邩�`�F�b�N���鏈��
//=============================================================================
void CGame::ReleaseCheckEnemyBullet(CBullet *pBullet, int *pNumBullet)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY)
	{
		pBullet->Uninit();
		pBullet = NULL;
		*pNumBullet = *pNumBullet - 1;
	}
}

//=============================================================================
// ��������f�[�^�ɕϊ����e�ɐݒ肷��
//=============================================================================
char *CGame::SetDataToEnemyBullet(CBullet *pBullet, char *pStr, int *pNumBullet)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY)
	{
		int nWord = 0;
		float fBulletPosX = 0.0f;
		float fBulletPosY = 0.0f;
		float fBulletPosZ = 0.0f;
		int nBulletIdx = 0;
		int nBulletType = 0;

		// �e�̍��W��ǂݎ��
		fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		// �e�̔ԍ���ǂݎ��
		nBulletIdx = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		// �e�̎�ނ�ǂݎ��
		nBulletType = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		// �e�Ƀf�[�^��ݒ�
		pBullet->SetPos(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ));
		pBullet->SetIdx(nBulletIdx);
		pBullet->SetType((CBullet::TYPE)nBulletType);
		*pNumBullet = *pNumBullet - 1;
	}

	return pStr;
}

//=============================================================================
// ��������f�[�^�ɕϊ������������G�̒e�ɐݒ肷��
//=============================================================================
char *CGame::SetDataToCreateEnemyBullet(char *pStr)
{
	int nWord = 0;
	float fBulletPosX = 0.0f;
	float fBulletPosY = 0.0f;
	float fBulletPosZ = 0.0f;
	int nBulletIdx = 0;
	int nBulletType = 0;

	// �e�̍��W��ǂݎ��
	fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̔ԍ���ǂݎ��
	nBulletIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̎�ނ�ǂݎ��
	nBulletType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�𐶐�����(�����ɕK�v�ȃf�[�^��ݒ�)
	CBulletEnemy *pBullet = CBulletEnemy::Create(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ), INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, NULL);
	pBullet->SetIdx(nBulletIdx);
	pBullet->SetType((CBullet::TYPE)nBulletType);

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^���A�C�e���ɐݒ肷�鏈��
//=============================================================================
char *CGame::SetDataToItemFromServer(char *pStr)
{
	int nWord = 0;
	int nNumItem = 0;

	// ��������Ă���e�̐���ǂݎ��
	nNumItem = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �����킹
	ReleaseItem(nNumItem);
	if (nNumItem == 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < ITEM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ITEM && nNumItem > 0)
			{// �A�C�e���N���X������
				pStr = SetDataToItem((CItem*)pScene, pStr, &nNumItem);
			}
			pScene = pSceneNext;
		}
	}

	// ����Ȃ��A�C�e���͐V���ɐ������f�[�^��ݒ�
	for (int nCntItem = 0; nCntItem < nNumItem; nCntItem++)
	{
		pStr = SetDataToCreateItem(pStr);
	}

	return pStr;
}

//=============================================================================
// ��������f�[�^�ɕϊ����A�C�e���ɐݒ肷��
//=============================================================================
char *CGame::SetDataToItem(CItem *pItem, char *pStr, int *pNumItem)
{
	int nWord = 0;
	float fItemPosX = 0.0f;
	float fItemPosY = 0.0f;
	float fItemPosZ = 0.0f;
	int nItemIdx = 0;
	int nItemType = 0;

	// �A�C�e���̍��W��ǂݎ��
	fItemPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fItemPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fItemPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �A�C�e���̔ԍ�
	nItemIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �A�C�e���̎�ޔԍ�
	nItemType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �A�C�e���̎�ނ��擾���Ⴄ�Ȃ���ւ���
	int nItemTypeOld = pItem->GetType();
	if (nItemType != nItemTypeOld)
	{
		// �j������
		pItem->Uninit();
		pItem = NULL;
		CScene::DeathCheck(ITEM_PRIORITY - 1);

		// �������Ȃ���
		pItem = CreateItem(D3DXVECTOR3(fItemPosX, fItemPosY, fItemPosZ), INITIALIZE_D3DXVECTOR3, nItemType);
		pItem->SetIdx(nItemIdx);
	}
	else
	{// ������ނ̃A�C�e���ł���
	    // �A�C�e���Ƀf�[�^��ݒ�
		pItem->SetPos(D3DXVECTOR3(fItemPosX, fItemPosY, fItemPosZ));
		pItem->SetIdx(nItemIdx);
	}
	*pNumItem = *pNumItem - 1;

	return pStr;
}

//=============================================================================
// ��������f�[�^�ɕϊ������������A�C�e���ɐݒ肷��
//=============================================================================
char *CGame::SetDataToCreateItem(char *pStr)
{
	int nWord = 0;
	float fItemPosX = 0.0f;
	float fItemPosY = 0.0f;
	float fItemPosZ = 0.0f;
	int nItemIdx = 0;
	int nItemType = 0;

	// �A�C�e���̍��W��ǂݎ��
	fItemPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fItemPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fItemPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �A�C�e���̔ԍ���ǂݎ��
	nItemIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �A�C�e���̎�ނ�ǂݎ��
	nItemType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �A�C�e���𐶐�����(�����ɕK�v�ȃf�[�^��ݒ�)
	CItem *pItem = CreateItem(D3DXVECTOR3(fItemPosX, fItemPosY, fItemPosZ), INITIALIZE_D3DXVECTOR3, nItemType);
	if (pItem != NULL)
	{
		pItem->SetIdx(nItemIdx);
	}

	return pStr;
}

//=============================================================================
// �A�C�e���̐������킹�鏈��
//=============================================================================
void CGame::ReleaseItem(int nNumItem)
{
	// ���݂̃A�C�e���̐���ݒ�
	int nAllItem = 0;
	nAllItem = CItem::GetNumAll();
	if (nAllItem < nNumItem)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < ITEM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ITEM && nAllItem > nNumItem)
			{// �A�C�e���N���X������
				ReleaseCheckItem((CItem*)pScene, &nAllItem);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck(ITEM_PRIORITY - 1);
}

//=============================================================================
// �A�C�e����j�����邩�`�F�b�N���鏈��
//=============================================================================
void CGame::ReleaseCheckItem(CItem *pItem, int *pNumItem)
{
	pItem->Uninit();
	pItem = NULL;
	*pNumItem = *pNumItem - 1;
}

//=============================================================================
// �Q�[���̏����u���b�N�̃f�[�^��ݒ�
//=============================================================================
char *CGame::SetDataToDeleteBlock(char *pStr)
{
	int nWord = 0;
	int nNumDeleteBlock = 0;
	int *pDeleteIdx = NULL;

	// �����u���b�N�̐���ǂݎ��
	nNumDeleteBlock = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumDeleteBlock <= 0) return pStr;

	// �������������������m��
	pDeleteIdx = new int[nNumDeleteBlock];
	for (int nCnt = 0; nCnt < nNumDeleteBlock; nCnt++)
	{
		pDeleteIdx[nCnt] = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
	}

	// �����������u���b�N���J������
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < BLOCK_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK && m_State == STATE_NORMAL)
			{// �u���b�N�N���X������
				ReleaseCheckBlock((CBlock*)pScene, pDeleteIdx, &nNumDeleteBlock);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck(BLOCK_PRIORITY - 1);

	// �������̊J��
	if (pDeleteIdx != NULL)
	{
		delete[] pDeleteIdx;
		pDeleteIdx = NULL;
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃u���b�N��j�����邩�`�F�b�N���鏈��
//=============================================================================
void CGame::ReleaseCheckBlock(CBlock *pBlock, int *pDeleteIdx, int *nNumDeleteBlock)
{
	int nIdx = pBlock->GetIdx();
	for (int nCntDeleteNum = 0; nCntDeleteNum < *nNumDeleteBlock; nCntDeleteNum++)
	{// ���������J��Ԃ�
		if (nIdx == pDeleteIdx[nCntDeleteNum] && pBlock != NULL)
		{// �ԍ���v
			pBlock->Uninit();
			pBlock = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̏����G�̃f�[�^��ݒ�
//=============================================================================
char *CGame::SetDataToDeleteEnemy(char *pStr)
{
	int nWord = 0;
	int nNumDeleteEnemy = 0;
	int nCntDelete = 0;
	int *pDeleteIdx = NULL;

	// �����G�̐���ǂݎ��
	nNumDeleteEnemy = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumDeleteEnemy <= 0) return pStr;

	// �������������������m��
	pDeleteIdx = new int[nNumDeleteEnemy];
	for (int nCnt = 0; nCnt < nNumDeleteEnemy; nCnt++)
	{
		pDeleteIdx[nCnt] = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
	}

	// �����������G���J������
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < (ENEMY_PRIORITY + 1); nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY && m_State == STATE_NORMAL)
			{// �G�N���X������
				ReleaseCheckDeleteEnemy((CEnemy*)pScene, pDeleteIdx, &nNumDeleteEnemy);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck(ENEMY_PRIORITY);

	// �������̊J��
	if (pDeleteIdx != NULL)
	{
		delete[] pDeleteIdx;
		pDeleteIdx = NULL;
	}

	return pStr;
}

//=============================================================================
// �Q�[���̓G��j�����邩�`�F�b�N���鏈��
//=============================================================================
void CGame::ReleaseCheckDeleteEnemy(CEnemy *pEnemy, int *pDeleteIdx, int *nNumDeleteEnemy)
{
	int nIdx = pEnemy->GetIdx();
	for (int nCntDeleteNum = 0; nCntDeleteNum < *nNumDeleteEnemy; nCntDeleteNum++)
	{// ���������J��Ԃ�
		if (nIdx == pDeleteIdx[nCntDeleteNum] && pEnemy != NULL)
		{// �ԍ���v
			pEnemy->SetDeathEffect();
			pEnemy->Uninit();
			pEnemy = NULL;
			if (m_pPowerMap != NULL)
			{
				m_pPowerMap->AddGauge();
			}
		}
	}
}

//=============================================================================
// �Q�[���̏����A�C�e���̃f�[�^��ݒ�
//=============================================================================
char *CGame::SetDataToDeleteItem(char *pStr)
{
	int nWord = 0;
	int nNumDeleteItem = 0;
	int nCntDelete = 0;
	int *pDeleteIdx = NULL;

	// �����G�̐���ǂݎ��
	nNumDeleteItem = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumDeleteItem <= 0) return pStr;

	// �������������������m��
	pDeleteIdx = new int[nNumDeleteItem];
	for (int nCnt = 0; nCnt < nNumDeleteItem; nCnt++)
	{
		pDeleteIdx[nCnt] = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
	}

	// �����������G���J������
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < ITEM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ITEM && m_State == STATE_NORMAL)
			{// �A�C�e���N���X������
				ReleaseCheckDeleteItem((CItem*)pScene, pDeleteIdx, &nNumDeleteItem);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck(ITEM_PRIORITY - 1);

	// �������̊J��
	if (pDeleteIdx != NULL)
	{
		delete[] pDeleteIdx;
		pDeleteIdx = NULL;
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃A�C�e����j�����邩�`�F�b�N���鏈��
//=============================================================================
void CGame::ReleaseCheckDeleteItem(CItem *pItem, int *pDeleteIdx, int *nNumDeleteItem)
{
	int nIdx = pItem->GetIdx();
	for (int nCntDeleteNum = 0; nCntDeleteNum < *nNumDeleteItem; nCntDeleteNum++)
	{// ���������J��Ԃ�
		if (nIdx == pDeleteIdx[nCntDeleteNum] && pItem != NULL)
		{// �ԍ���v
			pItem->Uninit();
			pItem = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̑���̏�Ԃ��擾���鏈��
//=============================================================================
char *CGame::SetDataToClientState(char *pStr)
{
	// ��Ԃ�ǂݎ��
	int nGameState = CFunctionLib::ReadInt(pStr, "");
	int nWord = 0;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// ��Ԃɂ���ă|�[�Y���邩�ݒ�
	if (nGameState == STATE_PAUSE && m_State == STATE_NORMAL && m_bPauseOpen == false)
	{
		m_State = STATE_PAUSE;
		CreateNotPause();
		CManager::GetSound()->PlaySound(GAME_SE_PAUSE_OPEN_IDX);
		CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX, true);
		m_bPauseOpen = true;
	}
	else if (nGameState == STATE_NORMAL && m_State == STATE_PAUSE && m_bPauseOpen == true)
	{
		if (m_pPause != NULL)
		{
			return pStr;
		}

		m_State = STATE_NORMAL;
		ReleaseNotPause();
		CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX, true);
		m_bPauseOpen = false;
	}
	else if (nGameState == STATE_END_RETRY)
	{
		m_State = STATE_END_RETRY;
		ReleaseNotPause();
	}
	else if (nGameState == STATE_END_QUIT)
	{
		m_State = STATE_END_QUIT;
		ReleaseNotPause();
	}

	return pStr;
}

//=============================================================================
// �Q�[���̑���̓|�����G�̐����擾���鏈��
//=============================================================================
char *CGame::SetDataToBreakEnemy(char *pStr)
{
	// �ԍ���ݒ�
	int nIdxClient = 0;
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		nIdxClient = pClient->GetClientId();
	}

	// �G��|���������擾
	int nNumBreak = 0;
	int nWord = 0;
	for (int nCntType = 0; nCntType < CEnemy::TYPE_MAX; nCntType++)
	{
		nNumBreak = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		m_nNumBreakEnemy[(nIdxClient + 1) % MAX_NUM_PLAYER][nCntType] = nNumBreak;
	}

	// �X�R�A���擾
	m_nScore[(nIdxClient + 1) % MAX_NUM_PLAYER] = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �ז������񐔂��擾
	m_nJammer[(nIdxClient + 1) % MAX_NUM_PLAYER] = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �N���[���ɂɓ��������񐔂��擾
	m_nCream[(nIdxClient + 1) % MAX_NUM_PLAYER] = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �A�C�e�����擾�����񐔂��擾
	m_nCollector[(nIdxClient + 1) % MAX_NUM_PLAYER] = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	return pStr;
}

//=============================================================================
// �Q�[���̓G�̃X�|�[�������擾���鏈��
//=============================================================================
char *CGame::SetDataToSpawnEnemyCount(char *pStr)
{
	// �X�|�[������ǂݎ��
	int nSpawn = CFunctionLib::ReadInt(pStr, "");
	int nWord = 0;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	if (nSpawn > m_nSpawnEnemyCount)
	{
		m_nSpawnEnemyCount = nSpawn;

		// UI�𐮂���
		CMap *pMap = GetMap();
		if (m_pUI != NULL && pMap != NULL)
		{
			m_pUI->ReCreateEnemyIcon(pMap->GetNumEnemyListData() - nSpawn);
		}
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃}�b�v�C�x���g���擾���鏈��
//=============================================================================
char *CGame::SetDataToMapEvent(char *pStr)
{
	// �}�b�v�C�x���g��ǂݎ��
	int nHinaEvent = CFunctionLib::ReadInt(pStr, "");
	int nWord = 0;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	int nNextEvent = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �C�x���g�̎�ނɂ���ď����킯
	int nHinaEventOld = m_HinaEvent;
	m_HinaEvent = (HINAEVENT)nHinaEvent;
	m_nNextEvent = nNextEvent;

	if (nHinaEventOld == HINAEVENT_NORMAL && m_HinaEvent == HINAEVENT_EVENTSTART)
	{
		CreateEventStartLogo();
	}
	else if (nHinaEventOld == HINAEVENT_EVENTSTART && m_HinaEvent == HINAEVENT_CHERRYBLOSSOMS)
	{
		CManager::GetSound()->PlaySound(GAME_BLOSSOMS_BGMIDX);
		ReleaseEventStartLogo();
	}
	else if (nHinaEventOld == HINAEVENT_CHERRYBLOSSOMS && m_HinaEvent == HINAEVENT_NORMAL)
	{
		CManager::GetSound()->StopSound(GAME_BLOSSOMS_BGMIDX);
	}
	else if (nHinaEventOld == HINAEVENT_EVENTSTART && m_HinaEvent == HINAEVENT_DROP_HINAARARE)
	{
		CManager::GetSound()->PlaySound(GAME_HINAARARE_BGMIDX);
		ReleaseEventStartLogo();
	}
	else if (nHinaEventOld == HINAEVENT_DROP_HINAARARE && m_HinaEvent == HINAEVENT_NORMAL)
	{
		CManager::GetSound()->StopSound(GAME_HINAARARE_BGMIDX);
	}

	return pStr;
}

//=============================================================================
// �Q�[���̑���v���C���[�̒e�ɓ����������ǂ����擾����
//=============================================================================
char *CGame::SetDataToHitBullet(char *pStr)
{
	// �����������ǂ����ǂݎ��
	m_bHitBulletFlag = CFunctionLib::ReadBool(pStr, "");
	int nWord = 0;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	int nIdxClient = 0;
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		nIdxClient = pClient->GetClientId();
	}

	if (m_bHitBulletFlag == true && m_pPlayer[nIdxClient] != NULL)
	{// �e�ɓ������Ă���
		if (m_pPlayer[nIdxClient]->GetState() != CPlayer::STATE_STOP)
		{// �X�g�b�v��Ԃł͂Ȃ�
			m_pPlayer[nIdxClient]->SetState(CPlayer::STATE_STOP);
			m_pPlayer[nIdxClient]->SetStateCounter(0);
		}
	}

	return pStr;
}

//=============================================================================
// �Q�[���̎i�ߕ��ɒe�ɓ����������ǂ����擾����
//=============================================================================
char *CGame::SetDataToHitHeadQuarters(char *pStr)
{
	// �����������ǂ����ǂݎ��
	m_bHitHeadQuarters = CFunctionLib::ReadBool(pStr, "");
	int nWord = 0;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	if (m_bHitHeadQuarters == true)
	{// �e�ɓ������Ă���
		m_State = STATE_GAMEOVER;
	}

	return pStr;
}


//*****************************************************************************
//
// ��Ԃɂ�鏈�������p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̃X�e�[�W�Z���N�g��Ԃ̍X�V����
//=============================================================================
void CGame::StageSelectUpdate(void)
{
	// �w�i����
	if (m_pStageBg == NULL)
	{
		CreateStageBg();
	}

	// �X�e�[�W���S����
	if (m_pStageLogo == NULL)
	{
		CreateStageLogo();
	}

	// �X�e�[�W�ԍ�����
	if (m_pStageNumber == NULL)
	{
		CreateStageNumber();
	}


	int nIdxClient = 0;
	CClient *pClient = CManager::GetClient();
	if(pClient != NULL)
	{
		nIdxClient = pClient->GetClientId();
		if (nIdxClient != 0)
		{
			ReleaseStageNumber();
			CalcStageDigits();
			CreateStageNumber();
		}
	}

	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL || CManager::GetClient()->GetClientId() != 0) return;

	if (pKey->GetTrigger(DIK_W) == true || pKey->GetRepeat(DIK_W) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_0) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_0) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_16) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_16) == true)
	{// ������̓��͂����ꂽ
		if (m_nStageIdx < GAME_MAPSTAGE_MAX - 1)
		{
			m_nStageIdx++;
			m_nMapIdx++;
			ReleaseStageNumber();
			CalcStageDigits();
			CreateStageNumber();
		}
	}
	else if (pKey->GetTrigger(DIK_S) == true || pKey->GetRepeat(DIK_S) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_1) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_1) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_17) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_17) == true)
	{// �������̓��͂����ꂽ
		if (m_nStageIdx > 0)
		{
			m_nStageIdx--;
			m_nMapIdx--;
			ReleaseStageNumber();
			CalcStageDigits();
			CreateStageNumber();
		}
	}
	else if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// ����{�^���������ꂽ
		SetState(STATE_STAGE_DISP);
		ReleaseMap();
		DeleteGameObject();
		CreateMap();
		m_nGameCounter = 0;
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W�ԍ���\�������Ԃ̍X�V����
//=============================================================================
void CGame::StageDispUpdate(void)
{
	// �Q�[�������U���g�ɕK�v�Ȃ��̂�j������
	ReleaseGameResult();

	// �w�i����
	if (m_pStageBg == NULL)
	{
		CreateStageBg();
	}

	// �X�e�[�W���S����
	if (m_pStageLogo == NULL)
	{
		CreateStageLogo();
	}

	// �X�e�[�W�ԍ�����
	if (m_pStageNumber == NULL)
	{
		CreateStageNumber();
	}

	// �J�E���^�[���Z
	m_nStateCounter++;
	if (m_nStateCounter % GAME_STAGEDISP_TIME == 0)
	{// �X�e�[�W�ԍ����o���Ă��炠����x������
		SetState(STATE_NORMAL);
		ReleaseStageBg();
		ReleaseStageLogo();
		ReleaseStageNumber();
		CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX);
	}
}

//=============================================================================
// �Q�[���̒ʏ��Ԃ̍X�V����
//=============================================================================
void CGame::NormalUpdate(void)
{
	// �N���[���Ƀ`�F�b�N����
	MistCheck();

	m_bPauseOpen = false;

	CInputKeyboard *pKey = CManager::GetKeyboard();
	CXInput *pXInput = CManager::GetXInput();
	if (pKey == NULL) return;

	// �|�[�YON/OFF�؂�ւ�
	if (pKey->GetTrigger(DIK_P) == true)
	{// �L�[�{�[�h����
		m_State = STATE_PAUSE;
		CreatePause();
		m_bPauseOpen = true;
		CManager::GetSound()->PlaySound(GAME_SE_PAUSE_OPEN_IDX);
		CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX, true);
		CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX, true);
		CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX + 1, true);
		CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX + 2, true);
		CManager::GetSound()->StopSound(GAME_BLOSSOMS_BGMIDX);
		CManager::GetSound()->StopSound(GAME_HINAARARE_BGMIDX);
	}
	else if (pXInput != NULL)
	{// �R���g���[���[����
		for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{
			if (pXInput->GetTrigger(nCntPlayer, CXInput::XIJS_BUTTON_4) == true)
			{
				m_State = STATE_PAUSE;
				m_bPauseOpen = true;
				CreatePause(nCntPlayer);
				CManager::GetSound()->PlaySound(GAME_SE_PAUSE_OPEN_IDX);
				CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX, true);
				CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX, true);
				CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX + 1, true);
				CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX + 2, true);
				CManager::GetSound()->StopSound(GAME_BLOSSOMS_BGMIDX);
				CManager::GetSound()->StopSound(GAME_HINAARARE_BGMIDX);
			}
		}
	}

#ifdef GAME_DEBUG  // �f�o�b�O�R�}���h�K�p��
	// �X�e�[�W�Z���N�g�f�o�b�O
	int nIdxClient = 0;
	if (CManager::GetClient() != NULL)
	{
		nIdxClient = CManager::GetClient()->GetClientId();
	}
	if (nIdxClient == 0)
	{
		if (pKey->GetTrigger(DIK_1) == true && pKey->GetPress(DIK_LSHIFT) == true)
		{
			SetState(STATE_PREV_MAP);
			StopBGM();
		}
		else if (pKey->GetTrigger(DIK_2) == true && pKey->GetPress(DIK_LSHIFT) == true)
		{
			SetState(STATE_NEXT_MAP);
			StopBGM();
		}
		else if (pKey->GetTrigger(DIK_3) == true && pKey->GetPress(DIK_LSHIFT) == true)
		{
			SetState(STATE_STAGE_SELECT);
			StopBGM();
		}
	}

	// ��ԃ`�F�b�N�f�o�b�O
	if (pKey->GetTrigger(DIK_4) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		SetState(STATE_GAMEOVER);
	}
	else if (pKey->GetTrigger(DIK_5) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		SetState(STATE_END);
	}

	// �v���C���[��|���f�o�b�O
	if (pKey->GetTrigger(DIK_6) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		if (m_pPlayer[nIdxClient] != NULL)
		{
			m_pPlayer[nIdxClient]->SetDeathEffect();
			m_pPlayer[nIdxClient]->Uninit();
			m_pPlayer[nIdxClient] = NULL;
		}
	}


	// �A�C�e���C�x���g�f�o�b�O
	if (pKey->GetTrigger(DIK_F1) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Star(0);
	}
	else if (pKey->GetTrigger(DIK_F2) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Star(1);
	}
	else if (pKey->GetTrigger(DIK_F3) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Grenade(0);
	}
	else if (pKey->GetTrigger(DIK_F4) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_1Up(0);
	}
	else if (pKey->GetTrigger(DIK_F5) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_1Up(1);
	}
	else if (pKey->GetTrigger(DIK_F6) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Scoop(0);
	}
	else if (pKey->GetTrigger(DIK_F7) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Clock(0);
	}
	else if (pKey->GetTrigger(DIK_F8) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Helmet(0);
	}
	else if (pKey->GetTrigger(DIK_F9) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Helmet(1);
	}

	// �X�e�[�W�C�x���g�f�o�b�O
	if (CManager::GetKeyboard()->GetTrigger(DIK_1) == true && CManager::GetKeyboard()->GetPress(DIK_RSHIFT) == true)
	{
		m_nNextEvent = HINAEVENT_CHERRYBLOSSOMS;
		m_HinaEvent = HINAEVENT_EVENTSTART;
		m_nEventCounter = 0;
		m_nNotEventCounter = 0;
		CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + (m_nNextEvent - 1));
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_2) == true && CManager::GetKeyboard()->GetPress(DIK_RSHIFT) == true)
	{
		m_nNextEvent = HINAEVENT_DROP_ITEM;
		m_HinaEvent = HINAEVENT_EVENTSTART;
		m_nEventCounter = 0;
		m_nNotEventCounter = 0;
		CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + (m_nNextEvent - 1));
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_3) == true && CManager::GetKeyboard()->GetPress(DIK_RSHIFT) == true)
	{
		m_nNextEvent = HINAEVENT_DROP_HINAARARE;
		m_HinaEvent = HINAEVENT_EVENTSTART;
		m_nEventCounter = 0;
		m_nNotEventCounter = 0;
		CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + (m_nNextEvent - 1));
	}

	CDebugProc::Print(1, "�f�o�b�O�R�}���h\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [1]�L�[        : �O�̃X�e�[�W�ɂ���\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [2]�L�[        : ���̃X�e�[�W�ɂ���\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [3]�L�[        : �X�e�[�W�Z���N�g��Ԃɂ���\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [4]�L�[        : �Q�[���I�[�o�[��Ԃɂ���\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [5]�L�[        : �I����Ԃɂ���\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [6]�L�[        : �v���C���[��|���Ďc�@�����炷\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [F1]�`[F2]�L�[ : �X�^�[�A�C�e���C�x���g\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [F3]�L�[       : �O���l�[�h�A�C�e���C�x���g\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [F4]�`[F5]�L�[ : 1Up�A�C�e���C�x���g\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [F6]�L�[       : �X�R�b�v�A�C�e���C�x���g\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [F7]�L�[       : ���v�A�C�e���C�x���g\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [F8]�`[F9]�L�[ : �w�����b�g�A�C�e���C�x���g\n");
	CDebugProc::Print(1, "[RSHIFT]�L�[ + [1]�L�[        : ������}�b�v�C�x���g�J�n\n");
	CDebugProc::Print(1, "[RSHIFT]�L�[ + [2]�L�[        : �A�C�e�������}�b�v�C�x���g�J�n\n");
	CDebugProc::Print(1, "[RSHIFT]�L�[ + [3]�L�[        : �ЂȂ���꓊���}�b�v�C�x���g�J�n\n\n");

#endif

	CDebugProc::Print(1, "�ʏ���\n");
}

//=============================================================================
// �Q�[���̃|�[�Y��Ԃ̍X�V����
//=============================================================================
void CGame::PauseUpdate(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CXInput *pXInput = CManager::GetXInput();
	if (pKey == NULL && pXInput == NULL) return;

	// �|�[�Y�̍X�V
	if (m_pPause != NULL)
	{
		m_pPause->Update();
	}

	// �|�[�YON/OFF�؂�ւ�
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{// 1�l�v���C�Ȃ��
		if (pKey->GetTrigger(DIK_P) == true || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_4))
		{
			m_State = STATE_NORMAL;
			ReleasePause();
			CScene::DeathCheck();
			CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX, true);

			// �}�b�v�C�x���g�ҋ@��ԂȂ�Ύ��̃C�x���g�ɂ���ď�����������
			if (m_HinaEvent == HINAEVENT_EVENTSTART)
			{
				switch (m_nNextEvent)
				{
				case HINAEVENT_CHERRYBLOSSOMS:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX, true);
					break;
				case HINAEVENT_DROP_ITEM:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + 1, true);
					break;
				case HINAEVENT_DROP_HINAARARE:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + 2, true);
					break;
				}
			}

			// �}�b�v�C�x���g����������Đ����Ȃ���
			switch (m_HinaEvent)
			{
			case HINAEVENT_CHERRYBLOSSOMS:
				CManager::GetSound()->PlaySound(GAME_BLOSSOMS_BGMIDX);
				break;
			case HINAEVENT_DROP_ITEM:
				break;
			case HINAEVENT_DROP_HINAARARE:
				CManager::GetSound()->PlaySound(GAME_HINAARARE_BGMIDX);
				break;
			}
		}
	}
	else if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL2P)
	{// ���[�J��2�l�v���C�Ȃ��
		// �ǂ���̃v���C���[���|�[�Y���J�����̂��擾
		int nOpneId = 0;
		if (m_pPause != NULL)
		{
			nOpneId = m_pPause->GetOpenId();
		}

		// �|�[�Y��������
		if (pKey->GetTrigger(DIK_P) == true || pXInput->GetTrigger(nOpneId, CXInput::XIJS_BUTTON_4))
		{
			m_State = STATE_NORMAL;
			ReleasePause();
			CScene::DeathCheck();
			CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX, true);

			// �}�b�v�C�x���g�ҋ@��ԂȂ�Ύ��̃C�x���g�ɂ���ď�����������
			if (m_HinaEvent == HINAEVENT_EVENTSTART)
			{
				switch (m_nNextEvent)
				{
				case HINAEVENT_CHERRYBLOSSOMS:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX, true);
					break;
				case HINAEVENT_DROP_ITEM:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + 1, true);
					break;
				case HINAEVENT_DROP_HINAARARE:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + 2, true);
					break;
				}
			}

			// �}�b�v�C�x���g����������Đ����Ȃ���
			switch (m_HinaEvent)
			{
			case HINAEVENT_CHERRYBLOSSOMS:
				CManager::GetSound()->PlaySound(GAME_BLOSSOMS_BGMIDX);
				break;
			case HINAEVENT_DROP_ITEM:
				break;
			case HINAEVENT_DROP_HINAARARE:
				CManager::GetSound()->PlaySound(GAME_HINAARARE_BGMIDX);
				break;
			}
		}
	}
	else if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C��2�l�v���C�Ȃ��
		// �|�[�Y�𐶐����Ă��Ȃ���Ώ������Ȃ�
		if (m_pPause == NULL)
		{
			return;
		}

		// �|�[�Y��������
		if (pKey->GetTrigger(DIK_P) == true || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_4))
		{
			m_State = STATE_NORMAL;
			ReleasePause();
			CScene::DeathCheck();
			CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX, true);

			// �}�b�v�C�x���g�ҋ@��ԂȂ�Ύ��̃C�x���g�ɂ���ď�����������
			if (m_HinaEvent == HINAEVENT_EVENTSTART)
			{
				switch (m_nNextEvent)
				{
				case HINAEVENT_CHERRYBLOSSOMS:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX, true);
					break;
				case HINAEVENT_DROP_ITEM:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + 1, true);
					break;
				case HINAEVENT_DROP_HINAARARE:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + 2, true);
					break;
				}
			}

			// �}�b�v�C�x���g����������Đ����Ȃ���
			switch (m_HinaEvent)
			{
			case HINAEVENT_CHERRYBLOSSOMS:
				CManager::GetSound()->PlaySound(GAME_BLOSSOMS_BGMIDX);
				break;
			case HINAEVENT_DROP_ITEM:
				break;
			case HINAEVENT_DROP_HINAARARE:
				CManager::GetSound()->PlaySound(GAME_HINAARARE_BGMIDX);
				break;
			}
		}
	}
}

//=============================================================================
// �Q�[���̃Q�[���I�[�o�[��Ԃ̍X�V����
//=============================================================================
void CGame::GameOverUpdate(void)
{
	// �Q�[���I�[�o�[�̃��S���o��
	if (m_pGameOverLogo == NULL)
	{
		CreateGameOverLogo();
		return;
	}

	// BGM���~�߂�
	StopBGM();

	// �Q�[���I�[�o�[���S�|���S�����グ��
	GameOverLogoUp();

	// �J�E���^�[���Z
	m_nStateCounter++;
	if (m_nStateCounter % GAME_GAMEOVER_TIMING == 0)
	{
		CFade *pFade = CManager::GetFade();
		if (pFade == NULL) return;
		if (pFade->GetFade() != CFade::FADE_NONE) return;

		// ���̃��[�h�ɐݒ�
		pFade->SetFade(CManager::MODE_TITLE);
	}

	CDebugProc::Print(1, "�Q�[���I�[�o�[���\n");
}

//=============================================================================
// �Q�[���̃Q�[���I�[�o�[���S����ɂ����鏈��
//=============================================================================
void CGame::GameOverLogoUp(void)
{
	if (m_pGameOverLogo != NULL)
	{
		D3DXVECTOR3 GameOverLogoPos = m_pGameOverLogo->GetPos();
		GameOverLogoPos.y -= GAME_GAMEOVER_POSUP;
		if (GameOverLogoPos.y <= m_pGameOverLogo->GetHeight() + GAME_GAMEOVER_POSYMAX)
		{
			GameOverLogoPos.y += GAME_GAMEOVER_POSUP;
		}

		m_pGameOverLogo->SetPos(GameOverLogoPos);
		m_pGameOverLogo->SetVtxBuffPos();
	}
}

//=============================================================================
// �Q�[���̃��U���g�\����Ԃ̍X�V����
//=============================================================================
void CGame::ResultUpdate(void)
{
	CDebugProc::Print(1, "�Q�[�������U���g���\n");
	CDebugProc::Print(1, "%d\n", m_nStateCounter);

	// �����~�߂Ă���
	CManager::GetSound()->StopSound(GAME_BLOSSOMS_BGMIDX);
	CManager::GetSound()->StopSound(GAME_HINAARARE_BGMIDX);

	// �i�s�x�ɂ���ď����킯
	switch (m_GameResultAddvance)
	{
	case GAMERESULT_NONE:
		ResultUpdate_None();
		break;
	case GAMERESULT_NORMALENEMY:
		ResultUpdate_Normal();
		break;
	case GAMERESULT_ARMOREENEMY:
		ResultUpdate_Armore();
		break;
	case GAMERESULT_FASTENEMY:
		ResultUpdate_Fast();
		break;
	case GAMERESULT_HEAVYENEMY:
		ResultUpdate_Heavy();
		break;
	case GAMERESULT_TORTAL:
		ResultUpdate_Tortal();
		break;
	}
}

//=============================================================================
// �Q�[���̉������Ȃ��Ƃ��̃Q�[�������U���g�X�V����
//=============================================================================
void CGame::ResultUpdate_None(void)
{
	m_nStateCounter++;
	if (m_nStateCounter % GAME_NEXTSCORE_TIMING == 0)
	{// ���̃X�R�A���o���^�C�~���O�ɂȂ���
		// �i�s�x�����Z
		m_GameResultAddvance = (GAMERESULT)(m_GameResultAddvance + 1);

		// �G�̃X�R�A�𐶐�
		CreateGameResult_EnemyScore_Number();

		// �J�E���^�[���Z�b�g
		m_nStateCounter = 0;
		m_bNextGameResult = false;

		// ����炷
		CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
	}
}

//=============================================================================
// �Q�[���̒ʏ�̃^���N�X�R�A�i�s�x�̃Q�[�������U���g�X�V����
//=============================================================================
void CGame::ResultUpdate_Normal(void)
{
	int nNumBreak[MAX_NUM_PLAYER] = {};
	bool bNextResult[MAX_NUM_PLAYER] = { false, false };

	// �J�E���^�[���Z
	m_nStateCounter++;
	if (m_nStateCounter % GAME_SCOREUP_TIMING == 0)
	{// �X�R�A���A�b�v������^�C�~���O������
		for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
		{
			if (m_apNumBreakEnemy[nCnt][CEnemy::TYPE_NORMAL] != NULL)
			{// �G����������Ă���
			    // ���݂̐������擾
				nNumBreak[nCnt] = m_apNumBreakEnemy[nCnt][CEnemy::TYPE_NORMAL]->GetNumber();

				if (nNumBreak[nCnt] < m_nNumBreakEnemy[nCnt][CEnemy::TYPE_NORMAL])
				{// ���݂̐������|��������菬����
					// ���̃��[�h�ɍs���Ȃ��悤�Ƀt���O������
					m_bNextGameResult = false;

					// �����𑝂₷
					nNumBreak[nCnt]++;

					// �\�����鐔����ݒ�
					m_apNumBreakEnemy[nCnt][CEnemy::TYPE_NORMAL]->SetNumber(nNumBreak[nCnt]);

					// �J�E���^�[���Z�b�g
					m_nStateCounter = 0;

					// ����炷
					CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
				}
				else
				{// ���݂̐������|�������ɂȂ��Ă���
					bNextResult[nCnt] = true;
				}
			}
			else
			{// ��������ĂȂ���Ύ��̍X�V�֍s�����Ԃ�
				bNextResult[nCnt] = true;
			}

			// �\������X�R�A���ς���
			if (m_apEnemyScore[nCnt][CEnemy::TYPE_NORMAL] != NULL)
			{
				// �\�����鐔����ݒ�
				m_apEnemyScore[nCnt][CEnemy::TYPE_NORMAL]->SetNumber(nNumBreak[nCnt] * ENEMY_SCORE_NORMAL);
			}
		}
	}

	// �܂��X�R�A�𑝂₵�Ă���Ȃ炱�̐�͏������Ȃ�
	if (bNextResult[0] == true && bNextResult[MAX_NUM_PLAYER - 1] == true)
	{
		m_bNextGameResult = true;
	}
	if (m_bNextGameResult == false) { return; }

	// �J�E���^�[���Z
	if (m_nStateCounter % GAME_NEXTSCORE_TIMING == 0)
	{// ���̃X�R�A���o���^�C�~���O�ɂȂ���
	    // �i�s�x�����Z
		m_GameResultAddvance = (GAMERESULT)(m_GameResultAddvance + 1);

		// �G�X�R�A�𐶐�
		CreateGameResult_EnemyScore_Number();

		// �J�E���^�[���Z�b�g
		m_nStateCounter = 0;

		// ����炷
		CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
	}
}

//=============================================================================
// �Q�[���̑��b�ԃ^���N�X�R�A�i�s�x�̃Q�[�������U���g�X�V����
//=============================================================================
void CGame::ResultUpdate_Armore(void)
{
	int nNumBreak[MAX_NUM_PLAYER] = {};
	bool bNextResult[MAX_NUM_PLAYER] = { false, false };

	// �J�E���^�[���Z
	m_nStateCounter++;
	if (m_nStateCounter % GAME_SCOREUP_TIMING == 0)
	{// �X�R�A���A�b�v������^�C�~���O������
		for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
		{
			if (m_apNumBreakEnemy[nCnt][CEnemy::TYPE_ARMORE] != NULL)
			{// �G����������Ă���
			    // ���݂̐������擾
				nNumBreak[nCnt] = m_apNumBreakEnemy[nCnt][CEnemy::TYPE_ARMORE]->GetNumber();

				if (nNumBreak[nCnt] < m_nNumBreakEnemy[nCnt][CEnemy::TYPE_ARMORE])
				{// ���݂̐������|��������菬����
				    // ���̃��[�h�ɍs���Ȃ��悤�Ƀt���O������
					m_bNextGameResult = false;

					// �����𑝂₷
					nNumBreak[nCnt]++;

					// �\�����鐔����ݒ�
					m_apNumBreakEnemy[nCnt][CEnemy::TYPE_ARMORE]->SetNumber(nNumBreak[nCnt]);

					// �J�E���^�[���Z�b�g
					m_nStateCounter = 0;

					// ����炷
					CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
				}
				else
				{// ���݂̐������|�������ɂȂ��Ă���
					bNextResult[nCnt] = true;
				}
			}
			else
			{// ��������ĂȂ���Ύ��̍X�V�֍s�����Ԃ�
				bNextResult[nCnt] = true;
			}

			// �\������X�R�A���ς���
			if (m_apEnemyScore[nCnt][CEnemy::TYPE_ARMORE] != NULL)
			{
				// �\�����鐔����ݒ�
				m_apEnemyScore[nCnt][CEnemy::TYPE_ARMORE]->SetNumber(nNumBreak[nCnt] * ENEMY_SCORE_ARMORE);
			}
		}
	}

	// �܂��X�R�A�𑝂₵�Ă���Ȃ炱�̐�͏������Ȃ�
	if (bNextResult[0] == true && bNextResult[MAX_NUM_PLAYER - 1] == true)
	{
		m_bNextGameResult = true;
	}
	if (m_bNextGameResult == false) { return; }

	if (m_nStateCounter % GAME_NEXTSCORE_TIMING == 0)
	{// ���̃X�R�A���o���^�C�~���O�ɂȂ���
	    // �i�s�x�����Z
		m_GameResultAddvance = (GAMERESULT)(m_GameResultAddvance + 1);

		// �G�X�R�A�𐶐�
		CreateGameResult_EnemyScore_Number();

		// �J�E���^�[���Z�b�g
		m_nStateCounter = 0;

		// ����炷
		CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
	}
}

//=============================================================================
// �Q�[���̑��˖C�^���N�X�R�A�i�s�x�̃Q�[�������U���g�X�V����
//=============================================================================
void CGame::ResultUpdate_Fast(void)
{
	int nNumBreak[MAX_NUM_PLAYER] = {};
	bool bNextResult[MAX_NUM_PLAYER] = { false, false };

	// �J�E���^�[���Z
	m_nStateCounter++;
	if (m_nStateCounter % GAME_SCOREUP_TIMING == 0)
	{// �X�R�A���A�b�v������^�C�~���O������
		for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
		{
			if (m_apNumBreakEnemy[nCnt][CEnemy::TYPE_FAST] != NULL)
			{// �G����������Ă���
			    // ���݂̐������擾
				nNumBreak[nCnt] = m_apNumBreakEnemy[nCnt][CEnemy::TYPE_FAST]->GetNumber();

				if (nNumBreak[nCnt] < m_nNumBreakEnemy[nCnt][CEnemy::TYPE_FAST])
				{// ���݂̐������|��������菬����
				    // ���̃��[�h�ɍs���Ȃ��悤�Ƀt���O������
					m_bNextGameResult = false;

					// �����𑝂₷
					nNumBreak[nCnt]++;

					// �\�����鐔����ݒ�
					m_apNumBreakEnemy[nCnt][CEnemy::TYPE_FAST]->SetNumber(nNumBreak[nCnt]);

					// �J�E���^�[���Z�b�g
					m_nStateCounter = 0;

					// ����炷
					CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
				}
				else
				{// ���݂̐������|�������ɂȂ��Ă���
					bNextResult[nCnt] = true;
				}
			}
			else
			{// ��������ĂȂ���Ύ��̍X�V�֍s�����Ԃ�
				bNextResult[nCnt] = true;
			}

			// �\������X�R�A���ς���
			if (m_apEnemyScore[nCnt][CEnemy::TYPE_FAST] != NULL)
			{
				// �\�����鐔����ݒ�
				m_apEnemyScore[nCnt][CEnemy::TYPE_FAST]->SetNumber(nNumBreak[nCnt] * ENEMY_SCORE_FAST);
			}
		}
	}

	// �܂��X�R�A�𑝂₵�Ă���Ȃ炱�̐�͏������Ȃ�
	if (bNextResult[0] == true && bNextResult[MAX_NUM_PLAYER - 1] == true)
	{
		m_bNextGameResult = true;
	}
	if (m_bNextGameResult == false) { return; }

	if (m_nStateCounter % GAME_NEXTSCORE_TIMING == 0)
	{// ���̃X�R�A���o���^�C�~���O�ɂȂ���
	    // �i�s�x�����Z
		m_GameResultAddvance = (GAMERESULT)(m_GameResultAddvance + 1);

		// �G�X�R�A�𐶐�
		CreateGameResult_EnemyScore_Number();

		// �J�E���^�[���Z�b�g
		m_nStateCounter = 0;

		// ����炷
		CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
	}
}

//=============================================================================
// �Q�[���̃w�r�[�^���N�X�R�A�i�s�x�̃Q�[�������U���g�X�V����
//=============================================================================
void CGame::ResultUpdate_Heavy(void)
{
	int nNumBreak[MAX_NUM_PLAYER] = {};
	bool bNextResult[MAX_NUM_PLAYER] = { false, false };

	// �J�E���^�[���Z
	m_nStateCounter++;
	if (m_nStateCounter % GAME_SCOREUP_TIMING == 0)
	{// �X�R�A���A�b�v������^�C�~���O������
		for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
		{
			if (m_apNumBreakEnemy[nCnt][CEnemy::TYPE_HEAVY] != NULL)
			{// �G����������Ă���
			    // ���݂̐������擾
				nNumBreak[nCnt] = m_apNumBreakEnemy[nCnt][CEnemy::TYPE_HEAVY]->GetNumber();

				if (nNumBreak[nCnt] < m_nNumBreakEnemy[nCnt][CEnemy::TYPE_HEAVY])
				{// ���݂̐������|��������菬����
			   	    // ���̃��[�h�ɍs���Ȃ��悤�Ƀt���O������
					m_bNextGameResult = false;

					// �����𑝂₷
					nNumBreak[nCnt]++;

					// �\�����鐔����ݒ�
					m_apNumBreakEnemy[nCnt][CEnemy::TYPE_HEAVY]->SetNumber(nNumBreak[nCnt]);

					// �J�E���^�[���Z�b�g
					m_nStateCounter = 0;

					// ����炷
					CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
				}
				else
				{// ���݂̐������|�������ɂȂ��Ă���
					bNextResult[nCnt] = true;
				}
			}
			else
			{// ��������ĂȂ���Ύ��̍X�V�֍s�����Ԃ�
				bNextResult[nCnt] = true;
			}

			// �\������X�R�A���ς���
			if (m_apEnemyScore[nCnt][CEnemy::TYPE_HEAVY] != NULL)
			{
				// �\�����鐔����ݒ�
				m_apEnemyScore[nCnt][CEnemy::TYPE_HEAVY]->SetNumber(nNumBreak[nCnt] * ENEMY_SCORE_HEAVY);
			}
		}
	}

	// �܂��X�R�A�𑝂₵�Ă���Ȃ炱�̐�͏������Ȃ�
	if (bNextResult[0] == true && bNextResult[MAX_NUM_PLAYER - 1] == true)
	{
		m_bNextGameResult = true;
	}
	if (m_bNextGameResult == false) { return; }


	if (m_nStateCounter % GAME_NEXTSCORE_TIMING == 0)
	{// ���̃X�R�A���o���^�C�~���O�ɂȂ���
	    // �i�s�x�����Z
		m_GameResultAddvance = (GAMERESULT)(m_GameResultAddvance + 1);

		// �g�[�^���X�R�A�𐶐�
		CreateGameResult_TortalScore_Number();

		// ����炷
		CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);

		// �J�E���^�[���Z�b�g
		m_nStateCounter = 0;
	}
}

//=============================================================================
// �Q�[���̃g�[�^���X�R�A�i�s�x�̃Q�[�������U���g�X�V����
//=============================================================================
void CGame::ResultUpdate_Tortal(void)
{
	// �J�E���^�[���Z
	m_nStateCounter++;
	if (m_nStateCounter % GAME_NEXTSCORE_TIMING == 0 && CTitle::GetGameMode() != CTitle::GAMEMODE_LOCAL1P &&
		m_bAddBonus == false)
	{// �{�[�i�X��t�^���Ă��Ȃ���Ԃŕt�^����^�C�~���O�ɂȂ���
		CreateGameResult_BonusScore();
	}

	if (m_nStateCounter % GAME_NEXTMODE_TIMING == 0)
	{
		if (m_nStageIdx >= GAME_MAPSTAGE_MAX - 1)
		{// ����ȏ�X�e�[�W���Ȃ�
			CFade *pFade = CManager::GetFade();
			if (pFade == NULL) return;
			if (pFade->GetFade() != CFade::FADE_NONE) return;

			// ���̃��[�h�ɐݒ�
			pFade->SetFade(CManager::MODE_RESULT);
		}
		else
		{// �܂����̃}�b�v�����݂���
		    // �N���C�A���g�ԍ����擾
			int nIdxClient = 0;
			if (CManager::GetClient() != NULL)
			{
				nIdxClient = CManager::GetClient()->GetClientId();
			}
			if (nIdxClient == 0)
			{// �z�X�g������
				SetState(STATE_CHANGE_MAP);
			}
		}
	}
}

//=============================================================================
// �Q�[���̃}�b�v��ς����Ԃ̍X�V����
//=============================================================================
void CGame::ChangeMapUpdate(void)
{
	// �N���C�A���g�ԍ����擾
	int nIdxClient = 0;
	if (CManager::GetClient() != NULL)
	{
		nIdxClient = CManager::GetClient()->GetClientId();
	}
	if (nIdxClient == 0)
	{// �z�X�g��������
		m_nMapIdx++;
		m_nStageIdx++;
		CalcStageDigits();
		ReleaseMap();
		DeleteGameObject();
		CreateMap();
		m_nGameCounter = 0;
		SetState(STATE_STAGE_DISP);
		CManager::GetSound()->PlaySound(GAME_SE_FANFARE_IDX);
	}
	else
	{// �z�X�g����Ȃ�������
		m_nMapIdx++;
		m_nStageIdx++;
		CalcStageDigits();
		ReleaseMap();
		DeleteGameObject();
		CreateMap();
		m_nGameCounter = 0;
		SetState(STATE_STAGE_DISP);
		CManager::GetSound()->PlaySound(GAME_SE_FANFARE_IDX);
	}

	// �f�[�^�N���A
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
}

//=============================================================================
// �Q�[���̏I����Ԃ̍X�V����
//=============================================================================
void CGame::EndUpdate(void)
{
	CDebugProc::Print(1, "�I�����\n");

	// �J�E���^�[���Z
	m_nStateCounter++;
	if (m_nStateCounter % GAME_RESULT_TIMING == 0)
	{// ������x���Ԃ�������
		SetState(STATE_RESULT);
		CreateGameResult();

		// ���y���~
		StopBGM();
	}
}

//=============================================================================
// �Q�[���̏I�����(RETRY)�̍X�V����
//=============================================================================
void CGame::EndRetryUpdate(void)
{
	CDebugProc::Print(1, "�|�[�Y����Q�[������蒼�����\n");

	CFade *pFade = CManager::GetFade();
	if (pFade == NULL) { return; }

	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		pFade->SetFade(CManager::MODE_GAME);

		// ���y���~
		StopBGM();
	}
}

//=============================================================================
// �Q�[���̏I�����(QUIT)�̍X�V����
//=============================================================================
void CGame::EndQuitUpdate(void)
{
	CDebugProc::Print(1, "�|�[�Y����^�C�g���ɖ߂���\n");

	CFade *pFade = CManager::GetFade();
	if (pFade == NULL) { return; }

	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		pFade->SetFade(CManager::MODE_TITLE);

		// ���y���~
		StopBGM();
	}
}

//=============================================================================
// �Q�[����BGM���~�����鏈��
//=============================================================================
void CGame::StopBGM(void)
{
	CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX);
	CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX, true);
	CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX + 1, true);
	CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX + 2, true);
	CManager::GetSound()->StopSound(GAME_BLOSSOMS_BGMIDX);
	CManager::GetSound()->StopSound(GAME_HINAARARE_BGMIDX);
}

//=============================================================================
// �Q�[���̃N���[���ɂ��������ǂ������肷�鏈��
//=============================================================================
void CGame::MistCheck(void)
{
	// �N���[���ɂ���������Ă��Ȃ���Ώ����I��
	if (m_pCreamMist == NULL) { return; }

	// �J�E���^�[�����炷
	m_nMistCounter++;
	if (m_nMistCounter >= GAME_CREAMMIST_APPAER_TIME)
	{// �N���[���ɂ��o�Ă����莞�Ԃ�����
		D3DXCOLOR col = m_pCreamMist->GetCol();
		col.a -= GAME_CREAMMIST_ALPHA_CUT;
		m_pCreamMist->SetCol(col);
		m_pCreamMist->SetVtxBuffCol();
		if (col.a <= 0.0f)
		{
			ReleaseCreamMist();
		}
	}
}

//=============================================================================
// �Q�[���̐��͐}�𑝂₷����
//=============================================================================
void CGame::AddPowerMap(void)
{
	if (m_pPowerMap == NULL) { return; }
	m_pPowerMap->AddGauge();
}


//*****************************************************************************
//
// �A�C�e���C�x���g�����p�֐�
// Auther : Hodaka Niwa & Jukiya Hayakawa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̃X�^�[�A�C�e���̃C�x���g����
//=============================================================================
void CGame::ItemEvent_Star(int nPlayerNumber)
{
	// �A�r���e�B�ύX
	if (m_pPlayer[nPlayerNumber] != NULL)
	{
		m_pPlayer[nPlayerNumber]->SwitchAbility();
	}

	// �X�R�A�����Z����
	m_nScore[nPlayerNumber] += GAME_ITEM_SCORE;

	// ����炷
	CManager::GetSound()->PlaySound(GAME_ITEMEVENT_SE_STAR);
}

//=============================================================================
// �Q�[���̃O���l�[�h�A�C�e���̃C�x���g����
//=============================================================================
void CGame::ItemEvent_Grenade(int nPlayerNumber)
{
	// ����������Ă���G������
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// �G������
				((CEnemy*)pScene)->SetDeathEffect();
				pScene->Uninit();
				pScene = NULL;
				AddPowerMap();
			}
			pScene = pSceneNext;
		}
	}

	// �J������h�炷
	ItemEvent_Granade_Shake();

	// �X�R�A�����Z����
	m_nScore[nPlayerNumber] += GAME_ITEM_SCORE;

	// ����炷
	CManager::GetSound()->PlaySound(GAME_ITEMEVENT_SE_GRENADE);
}

//=============================================================================
// �Q�[���̃O���l�[�h����������ɃJ������h�炷����
//=============================================================================
void CGame::ItemEvent_Granade_Shake(void)
{
	// �J������h�炷��Ԃɂ���
	if (GetCameraManager() == NULL) { return; }
	CCamera *pCamera = GetCameraManager()->GetCamera();
	if (pCamera == NULL) { return; }

	if (pCamera->GetType() == CCamera::TYPE_GAME)
	{// �Q�[���Ŏg�p���Ă���J�����ł���
		CGameCamera *pGameCamera = (CGameCamera*)pCamera;
		pGameCamera->StartShake(m_ItemEventData.nShakeTime, m_ItemEventData.fShake, m_ItemEventData.fShakeCut);
	}
}

//=============================================================================
// �Q�[����1Up�A�C�e���̃C�x���g����
//=============================================================================
void CGame::ItemEvent_1Up(int nPlayerNumber)
{
	// �v���C���[�̎c�@���𑝂₷
	if (m_nPlayerStock[nPlayerNumber] < m_nPlayerStockMax)
	{// �c�@�̍ő�l��菭�Ȃ�
		m_nPlayerStock[nPlayerNumber]++;

		// UI�̐����𑝂₷
		if (m_pUI != NULL)
		{
			m_pUI->ReCreatePlayerStock(&m_nPlayerStock[0]);
		}
	}

	// �X�R�A�����Z����
	m_nScore[nPlayerNumber] += GAME_ITEM_SCORE;

	// ����炷
	CManager::GetSound()->PlaySound(GAME_ITEMEVENT_SE_1UP);
}

//=============================================================================
// �Q�[���̃X�R�b�v�A�C�e���̃C�x���g����
//=============================================================================
void CGame::ItemEvent_Scoop(int nPlayerNumber)
{
	// �i�ߕ��̎���ɉ󂹂Ȃ��u���b�N��u��

	// �X�R�A�����Z����
	m_nScore[nPlayerNumber] += GAME_ITEM_SCORE;

	// ����炷
	CManager::GetSound()->PlaySound(GAME_ITEMEVENT_SE_SCOOP);
}

//=============================================================================
// �Q�[���̎��v�A�C�e���̃C�x���g����
//=============================================================================
void CGame::ItemEvent_Clock(int nPlayerNumber)
{
	// �G�������Ȃ���Ԃɂ���
	m_bEnemyMove = false;
	m_nEnemyMoveCounter = m_ItemEventData.nStop;

	// �X�R�A�����Z����
	m_nScore[nPlayerNumber] += GAME_ITEM_SCORE;

	// ����炷
	CManager::GetSound()->PlaySound(GAME_ITEMEVENT_SE_CLOCK);
}

//=============================================================================
// �Q�[���̃w�����b�g�A�C�e���̃C�x���g����
//=============================================================================
void CGame::ItemEvent_Helmet(int nPlayerNumber)
{
	// �X�R�A�����Z����
	m_nScore[nPlayerNumber] += GAME_ITEM_SCORE;

	// ����炷
	CManager::GetSound()->PlaySound(GAME_ITEMEVENT_SE_HELMET);
}


//*****************************************************************************
//
// �X�R�A���Z���s�p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̒ʏ�̃^���N��|�����������Z���鏈��
//=============================================================================
void CGame::ScoreUp_NormalEnemy(int nPlayerIdx)
{
	m_nNumBreakEnemy[nPlayerIdx][CEnemy::TYPE_NORMAL]++;
}

//=============================================================================
// �Q�[���̑��b�ԃ^���N��|�����������Z���鏈��
//=============================================================================
void CGame::ScoreUp_ArmoreEnemy(int nPlayerIdx)
{
	m_nNumBreakEnemy[nPlayerIdx][CEnemy::TYPE_ARMORE]++;
}

//=============================================================================
// �Q�[���̑��˖C�^���N��|�����������Z���鏈��
//=============================================================================
void CGame::ScoreUp_FastEnemy(int nPlayerIdx)
{
	m_nNumBreakEnemy[nPlayerIdx][CEnemy::TYPE_FAST]++;
}

//=============================================================================
// �Q�[���̃w�r�[�^���N��|�����������Z���鏈��
//=============================================================================
void CGame::ScoreUp_HeavyEnemy(int nPlayerIdx)
{
	m_nNumBreakEnemy[nPlayerIdx][CEnemy::TYPE_HEAVY]++;
}



//*****************************************************************************
//
// �}�b�v�C�x���g�p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̂ЂȍՂ�̃}�b�v�C�x���g����
//=============================================================================
void CGame::MapEvent_Hinamatsuri(void)
{
	CDebugProc::Print(1, "�ЂȍՂ�C�x���g\n");

	// �|�[�Y���Ȃ�X�V���Ȃ�
	if (m_State == STATE_PAUSE || m_State == STATE_END_RETRY || m_State == STATE_END_QUIT) { return; }

	// ���݂̃C�x���g��Ԃɂ���ď����킯
	switch (m_HinaEvent)
	{
	case HINAEVENT_NORMAL:
		MapEvent_Hinamatsuri_Normal();
		break;
	case HINAEVENT_CHERRYBLOSSOMS:
		MapEvent_Hinamatsuri_CherryBlossoms();
		break;
	case HINAEVENT_DROP_ITEM:
		MapEvent_Hinamatsuri_Drop_Item();
		break;
	case HINAEVENT_DROP_HINAARARE:
		MapEvent_Hinamatsuri_Drop_Hinaarare();
		break;
	case HINAEVENT_EVENTSTART:
		MapEvent_Hinamatsuri_EventStart();
		break;
	}
}

//=============================================================================
// �Q�[���̂ЂȍՂ�̒ʏ�̃}�b�v�C�x���g����
//=============================================================================
void CGame::MapEvent_Hinamatsuri_Normal(void)
{
	CDebugProc::Print(1, "�ʏ�C�x���g\n");

	// �I�����C���v���C�̏ꍇ�z�X�g����Ȃ��Ȃ珈�����Ȃ�
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		CClient *pClient = CManager::GetClient();
		if (pClient != NULL)
		{
			if (pClient->GetClientId() != 0) { return; }
		}
	}

	// �ʏ펞�łȂ���Ώ������Ȃ�
	if (m_State != STATE_NORMAL) { return; }

	// �C�x���g�J�E���^�[���Z
	m_nEventCounter++;
	m_nNotEventCounter++;
	if ((m_nRandomEventEva - m_nNotEventCounter) <= 0 || m_nEventCounter % (m_nRandomEventEva - m_nNotEventCounter) == 0)
	{// �����_���ŃC�x���g�������N����
		// �J�E���^�[������
		m_nNotEventCounter = 0;
		m_nEventCounter = 0;

		// �����N�����C�x���g�������_���Ō���
		m_nNextEvent = (HINAEVENT)(rand() % GAME_MAPEVENT_MAX) + 1;

		// �}�b�v�C�x���g�J�n��Ԃɐݒ�
		m_HinaEvent = HINAEVENT_EVENTSTART;

		// ����炷
		CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + (m_nNextEvent - 1));
	}
}

//=============================================================================
// �Q�[���̂ЂȍՂ�̍��������U��}�b�v�C�x���g����
//=============================================================================
void CGame::MapEvent_Hinamatsuri_CherryBlossoms(void)
{
	CDebugProc::Print(1, "�������U��C�x���g\n");
	if (m_State != STATE_NORMAL) { return; }

	// ���̉Ԃт琶��
	for (int nCnt = 0; nCnt < m_CherryBlossomsData.nLap; nCnt++)
	{
		CreateBlossoms();
	}

	// �C�x���g�J�E���^�[�i�s
	m_nEventCounter++;

	// �z�X�g�ł͂Ȃ��Ȃ珈�����Ȃ�
	int nIdxClient = 0;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		CClient *pClient = CManager::GetClient();
		if (pClient != NULL)
		{
			nIdxClient = pClient->GetClientId();
		}
	}
	if (nIdxClient != 0) { return; }

	if (m_nEventCounter >= m_CherryBlossomsData.nTime)
	{// �J�E���^�[�����l�ɂȂ���
		m_nEventCounter = 0;
		m_HinaEvent = HINAEVENT_NORMAL;

		// �����~�߂�
		CManager::GetSound()->StopSound(GAME_BLOSSOMS_BGMIDX);
	}
}

//=============================================================================
// �Q�[���̂ЂȍՂ�̃A�C�e���𓊉�����}�b�v�C�x���g����
//=============================================================================
void CGame::MapEvent_Hinamatsuri_Drop_Item(void)
{
	CDebugProc::Print(1, "�A�C�e���h���b�v�C�x���g\n");
	if (m_State != STATE_NORMAL) { return; }

	// �z�X�g�ł͂Ȃ��Ȃ珈�����Ȃ�
	int nIdxClient = 0;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		CClient *pClient = CManager::GetClient();
		if (pClient != NULL)
		{
			nIdxClient = pClient->GetClientId();
		}
	}
	if (nIdxClient != 0) { return; }

	// �~���̐���
	if (m_pItemCylinder == NULL)
	{
		// �A�C�e�����o���ʒu���}�X�ڂ��烉���_���Ōv�Z
		int nCntCheck = 0;
		do
		{// �삪����ꏊ�ȊO�ɂȂ����甲����
			int nMassX = rand() % MASS_BLOCK_X;
			int nMassZ = rand() % MASS_BLOCK_Z;
			m_ItemDropPos.x = (nMassX * MASS_SIZE_X) + MASS_SIZE_X_HALF - (MASS_BLOCK_X * MASS_SIZE_X_HALF);
			m_ItemDropPos.z = (nMassZ * MASS_SIZE_Z) + MASS_SIZE_Z_HALF - (MASS_BLOCK_Z * MASS_SIZE_Z_HALF);
			m_ItemDropPos.y = GAME_DROPITEM_POS_Y;
			nCntCheck++;
			if (nCntCheck == 3000)
			{// 3000��`�F�b�N���Ă��߂Ȃ甲����(�X�g�b�v�o�O�h�~)
				break;
			}
		} while (MapEvent_Hinamatsuri_Drop_Item_CheckPos() == true);

		// �~������
		m_pItemCylinder = CItemCylinder::Create(m_ItemDropPos, INITIALIZE_D3DXVECTOR3,
			GAME_DROPITEM_CYLINDER_COL, GAME_DROPITEM_CYLINDER_HEIGHT, GAME_DROPITEM_CYLINDER_RADIUS,
			GAME_DROPITEM_CYLINDER_XBLOCK, GAME_DROPITEM_CYLINDER_YBLOCK, GAME_DROPITEM_CYLINDER_PRIORITY);

		// ����炷
		CManager::GetSound()->PlaySound(GAME_DROPITEM_CYLINDER_SE_IDX);

		// �����������ŏI��������
		return;
	}

	// �~�����ړ�������
	D3DXVECTOR3 CylinderPos = m_pItemCylinder->GetPos();
	CylinderPos.y += GAME_DROPITEM_CYLINDER_MOVE_Y;
	m_pItemCylinder->SetPos(CylinderPos);
	if (CylinderPos.y > 0.0f)
	{
		return;
	}
	m_pItemCylinder->SetPos(D3DXVECTOR3(CylinderPos.x, 0.0f, CylinderPos.z));

	// �A�C�e���𐶐�����
	if (m_pItem == NULL)
	{
		// �A�C�e�����o����ނ�ݒ�
		int nItemType = rand() % CItem::TYPE_MAX;

		// �A�C�e������
		m_pItem = CreateItem(m_ItemDropPos, INITIALIZE_D3DXVECTOR3, (CItem::TYPE)nItemType);

		// ����炷
		CManager::GetSound()->PlaySound(GAME_DROPITEM_ITEM_SE_IDX);
	}

	// �A�C�e���̍��W���擾
	if (m_pItem == NULL)
	{
		m_nEventCounter = 0;
		m_HinaEvent = HINAEVENT_NORMAL;
		return;
	}
	D3DXVECTOR3 ItemPos = m_pItem->GetPos();

	// �A�C�e�������Ɉړ�������
	ItemPos.y += GAME_DROPITEM_ITEM_MOVE_Y;

	// �G�t�F�N�g�Ǌ��N���X���擾
	m_nEventCounter++;
	if (m_nEventCounter % GAME_DROPITEM_EFFECT_APPEAR == 0)
	{
		CEffectManager *pEffectManager = GetEffectManager();
		if (pEffectManager == NULL) { return; }

		// �G�t�F�N�g�𐶐�
		pEffectManager->SetEffect(ItemPos + D3DXVECTOR3(0.0f, 30.0f, 0.0f),
			INITIALIZE_D3DXVECTOR3, GAME_DROPITEM_EFFECT_IDX);
	}

	// �A�C�e�����n�ʂɂ��Ă��Ȃ���Ώ����I��
	if (ItemPos.y > 0.0f)
	{
		m_pItem->SetPos(ItemPos);
		return;
	}

	// �A�C�e���̍��W��n�ʂɖ߂�
	ItemPos.y = 0.0f;

	// �A�C�e���̍��W��ݒ�
	m_pItem->SetPos(ItemPos);

	// �V�����_�[�ɏ�Ԃ�ݒ�
	m_pItemCylinder->SetState(CItemCylinder::STATE_DEATH);
	m_pItemCylinder = NULL;

	// �����܂ŏ������ʂ�����ʏ�}�b�v�C�x���g��Ԃɖ߂�
	m_pItem = NULL;
	m_nEventCounter = 0;
	m_HinaEvent = HINAEVENT_NORMAL;
}


//=============================================================================
// �Q�[���̃A�C�e���𓊉�����ʒu�ɐ삪�Ȃ������肷�鏈��
//=============================================================================
bool CGame::MapEvent_Hinamatsuri_Drop_Item_CheckPos(void)
{
	bool bCol = false;

	CScene *pScene = NULL;               // �I�u�W�F�N�g�̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃I�u�W�F�N�g�̃|�C���^

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULL�ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_RIVER)
			{// �삾������
				bCol = MapEvent_Hinamatsuri_Drop_Item_CheckRiver((CRiver*)pScene);
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;
		}
	}

	return bCol;
}

//=============================================================================
// �Q�[���̃A�C�e���𓊉�����ʒu�ɐ삪�Ȃ������肷�鏈��
//=============================================================================
bool CGame::MapEvent_Hinamatsuri_Drop_Item_CheckRiver(CRiver *pRiver)
{
	if (pRiver != NULL)
	{
		CBoxCollider *pBoxCollider = pRiver->GetBoxCollider();
		if (pBoxCollider != NULL)
		{
			if (pBoxCollider->Collision(&m_ItemDropPos, &m_ItemDropPos, &INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, NULL) == true)
			{
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// �Q�[���̂ЂȍՂ�̂ЂȂ����𓊉�����}�b�v�C�x���g����
//=============================================================================
void CGame::MapEvent_Hinamatsuri_Drop_Hinaarare(void)
{
	CDebugProc::Print(1, "�ЂȂ����h���b�v�C�x���g\n");
	if (m_State != STATE_NORMAL) { return; }

	// �ЂȂ���꓊��
	int nAppear = m_HinarareData.nAppear;
	if (nAppear <= 0 || m_nEventCounter % nAppear == 0)
	{
		CreateHinaarare();
	}

	// �J������h�炷��Ԃɂ���
	if (GetCameraManager() == NULL) { return; }
	CCamera *pCamera = GetCameraManager()->GetCamera();
	if (pCamera == NULL) { return; }

	if (pCamera->GetType() == CCamera::TYPE_GAME)
	{// �Q�[���Ŏg�p���Ă���J�����ł���
		CGameCamera *pGameCamera = (CGameCamera*)pCamera;
		pGameCamera->StartShake(1, GAME_HINAARARE_CAMERA_SHAKE, GAME_HINAARARE_CAMERA_SHAKE);
	}

	// �C�x���g�J�E���^�[�i�s
	m_nEventCounter++;

	// �z�X�g�ł͂Ȃ��Ȃ珈�����Ȃ�
	int nIdxClient = 0;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		CClient *pClient = CManager::GetClient();
		if (pClient != NULL)
		{
			nIdxClient = pClient->GetClientId();
		}
	}
	if (nIdxClient != 0) { return; }

	if (m_nEventCounter >= m_HinarareData.nTime)
	{// �J�E���^�[�����l�ɂȂ���
		m_nEventCounter = 0;
		m_HinaEvent = HINAEVENT_NORMAL;

		// �����~�߂�
		CManager::GetSound()->StopSound(GAME_HINAARARE_BGMIDX);
	}
}

//=============================================================================
// �Q�[���̃}�b�v�C�x���g�J�n���̏���
//=============================================================================
void CGame::MapEvent_Hinamatsuri_EventStart(void)
{
	CDebugProc::Print(1, "�}�b�v�C�x���g�J�n\n");

	// �|���S���𐶐�����
	CreateEventStartLogo();

	// ��������Ă����牡�ɓ�����
	D3DXVECTOR3 logoPos = m_pEventStartLogo->GetPos();
	logoPos.x += GAME_MAPEVENT_STARTLOGO_MOVE;
	m_pEventStartLogo->SetPos(logoPos);
	m_pEventStartLogo->SetVtxBuffPos();

	// �z�X�g�ł͂Ȃ��Ȃ珈�����Ȃ�
	int nIdxClient = 0;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		CClient *pClient = CManager::GetClient();
		if (pClient != NULL)
		{
			nIdxClient = pClient->GetClientId();
		}
	}
	if (nIdxClient != 0) { return; }

	// �C�x���g�J�E���^�[�i�s
	m_nEventCounter++;
	if (m_nEventCounter >= GAME_MAPEVENT_STARTTIME)
	{// �J�E���^�[�����l�ɂȂ���
		m_nEventCounter = 0;
		m_HinaEvent = (HINAEVENT)m_nNextEvent;

		// �C�x���g�J�n���S���J��
		ReleaseEventStartLogo();

		// �炷�����J�n����C�x���g����ĕ�����
		switch (m_HinaEvent)
		{
		case HINAEVENT_CHERRYBLOSSOMS:
			CManager::GetSound()->PlaySound(GAME_BLOSSOMS_BGMIDX);
			break;
		case HINAEVENT_DROP_ITEM:
			break;
		case HINAEVENT_DROP_HINAARARE:
			CManager::GetSound()->PlaySound(GAME_HINAARARE_BGMIDX);
			break;
		}
	}
}


//*****************************************************************************
//
// �Q�[�����X�|�[�������p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̃v���C���[�����X�|�[���ʒu�ɂ��炷����
//=============================================================================
void CGame::SetPlayerPosToSpawn(void)
{
	// �v���C���[�̃��X�|�[���ʒu���擾
	D3DXVECTOR3 PlayerPos = INITIALIZE_D3DXVECTOR3;
	CMap *pMap = GetMap();
	if (pMap == NULL) { return; }

	// �v���C���[�̈ʒu�����X�|�[���ʒu�ɐݒ�
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C���v���C�Ȃ��
		PlayerPos = pMap->GetPlayerRespawn(CManager::GetClient()->GetClientId())->GetPos();
		if (m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
		{
			m_pPlayer[CManager::GetClient()->GetClientId()]->SetPos(PlayerPos);
			m_pPlayer[CManager::GetClient()->GetClientId()]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
			m_pPlayer[CManager::GetClient()->GetClientId()]->SetNowRotInfo(CCharacter::ROT_UP);
		}
	}
	else
	{// ���[�J���v���C�Ȃ��
		for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{// �v���C���[�̐l�����J��Ԃ�
			PlayerPos = pMap->GetPlayerRespawn(nCntPlayer)->GetPos();
			if (m_pPlayer[nCntPlayer] != NULL)
			{
				m_pPlayer[nCntPlayer]->SetPos(PlayerPos);
				m_pPlayer[nCntPlayer]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
				m_pPlayer[nCntPlayer]->SetNowRotInfo(CCharacter::ROT_UP);
			}
		}
	}
}

//=============================================================================
// �Q�[���̃v���C���[�����X�|�[�������邩�`�F�b�N����
//=============================================================================
void CGame::CheckPlayerResSpawn(int nCntPlayer)
{
	if (m_pPlayer[nCntPlayer] == NULL && m_nPlayerStock[nCntPlayer] >= 1)
	{
		m_nPlayerRespawnCounter++;
		if (m_nPlayerRespawnCounter % GAME_PLAYER_RESPAWN_TIMING == 0)
		{
			m_nPlayerRespawnCounter = 0;
			m_nPlayerStock[nCntPlayer]--;
			m_pPlayer[nCntPlayer] = m_pPlayerManager[nCntPlayer]->SetPlayer(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nCntPlayer);

			// �v���C���[�̃��X�|�[���ʒu���擾
			D3DXVECTOR3 PlayerPos = INITIALIZE_D3DXVECTOR3;
			CMap *pMap = GetMap();
			if (pMap != NULL)
			{
				PlayerPos = pMap->GetPlayerRespawn(nCntPlayer)->GetPos();
			}

			// �v���C���[�̈ʒu�����炷
			m_pPlayer[nCntPlayer]->SetPos(PlayerPos);

			// �G�t�F�N�g���o��
			D3DXVECTOR3 EffectPos = PlayerPos;
			CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
			if (pEffectManager != NULL)
			{
				pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, GAME_PLAYER_SPAWN_EFFECT_IDX);
				pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, GAME_PLAYER_SPAWN_EFFECT_IDX + 1);
			}

			// ����炷
			CManager::GetSound()->PlaySound(GAME_SE_PLAYER_RESPAWN_IDX);

			// UI�̐��������炷
			if (m_pUI != NULL)
			{
				m_pUI->ReCreatePlayerStock(&m_nPlayerStock[0]);
			}
		}
	}
}

//=============================================================================
// �Q�[���̓G�̐�������^�C�~���O���ǂ������肷��
//=============================================================================
void CGame::CheckEnemySpawn(int nTime)
{
	CMap *pMap = GetMap();
	if (pMap == NULL)return;

	int nNumEnemyListData = pMap->GetNumEnemyListData();
	CEnemy_ListData *pEnemyData = NULL;

	// �����G���o�Ȃ��̂ł���΂��̎��_�ŏ����I��
	if (m_nSpawnEnemyCount == nNumEnemyListData || m_State != STATE_NORMAL) return;

	// �J�E���^�[����
	m_nGameCounter++;
	for (int nCnt = 0; nCnt < nNumEnemyListData; nCnt++)
	{// �G�𐶐����鐔���J��Ԃ�
		pEnemyData = pMap->GetEnemyListData(nCnt);
		if (pEnemyData->GetRespawnTime() == nTime)
		{// ���X�|�[������^�C�~���O�ł���
			EnemySpawn(pMap, pEnemyData, nCnt);
			m_nSpawnEnemyCount++;
		}
	}
}

//=============================================================================
// �Q�[���̓G���X�|�[��������
//=============================================================================
void CGame::EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt)
{
	// �G�̃��X�|�[���ʒu���擾
	D3DXVECTOR3 EnemyPos = pMap->GetEnemyRespawn(pEnemyData->GetRespawnIdx())->GetPos();

	// �G�̐���
	CreateEnemy(EnemyPos, INITIALIZE_D3DXVECTOR3, pEnemyData);

	// UI�̕ύX
	if (m_pUI != NULL)
	{
		m_pUI->CutEnemyIcon();
	}
}


//*****************************************************************************
//
// �}�b�v��ς���p�֐�(�f�o�b�O�p�̊֐�����)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̊e��J�E���^�[������������
//=============================================================================
void CGame::ResetCounter(void)
{
	// �Q�[�����C�x���g�ɕK�v�ȕϐ���������
	m_nSpawnEnemyCount = 0;
	m_nGameCounter = 0;
	m_nStateCounter = 0;
	m_nEventCounter = 0;
	m_nEnemyMoveCounter = 0;
	m_HinaEvent = HINAEVENT_NORMAL;

	// �Q�[�����U���g�ɕK�v�ȕϐ���������
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
		{
			m_nNumBreakEnemy[nCntPlayer][nCntEnemy] = 0;
		}
	}

	// �Q�[�W�����Z�b�g
	if (m_pPowerMap != NULL)
	{
		m_pPowerMap->ResetGauge();
	}
}

//=============================================================================
// �Q�[���̎��̃}�b�v�ɐ؂�ւ��鏈��
//=============================================================================
void CGame::ChangeNextMap(void)
{
	// ���݂̃}�b�v��j��
	ReleaseMap();

	// �Q�[���Ɏg�p���Ă���I�u�W�F�N�g��j��
	DeleteGameObject();

	// �}�b�v�ԍ��𑝂₵�}�b�v��ǂݍ���
	m_nMapIdx++;
	m_nStageIdx++;
	if (m_nStageIdx >= GAME_MAPSTAGE_MAX)
	{
		m_nMapIdx--;
		m_nStageIdx--;
	}
	CreateMap();

	// �}�b�v�ԍ��̌������v�Z���Ă���
	CalcStageDigits();

	// �v���C���[�̈ʒu���X�|�[���ʒu��ݒ�
	SetPlayerPosToSpawn();

	// �e��J�E���^�[������
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;

	// ��Ԃ�ݒ�
	SetState(STATE_STAGE_DISP);
}

//=============================================================================
// �Q�[���̑O�̃}�b�v�ɐ؂�ւ��鏈��
//=============================================================================
void CGame::ChangePrevMap(void)
{
	// ���݂̃}�b�v��j��
	ReleaseMap();

	// �Q�[���Ɏg�p���Ă���I�u�W�F�N�g��j��
	DeleteGameObject();

	// �}�b�v�ԍ������炵�}�b�v��ǂݍ���
	m_nMapIdx--;
	m_nStageIdx--;
	if (m_nStageIdx < 0)
	{
		m_nMapIdx++;
		m_nStageIdx++;
	}
	CreateMap();

	// �}�b�v�ԍ��̌������v�Z���Ă���
	CalcStageDigits();

	// �v���C���[�̈ʒu���X�|�[���ʒu�ɐݒ�
	SetPlayerPosToSpawn();

	// �e��J�E���^�[������
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;

	// ��Ԃ�ݒ�
	SetState(STATE_STAGE_DISP);
}

//=============================================================================
// �Q�[���̃Q�[���Ɏg�p���Ă���I�u�W�F�N�g��j�����鏈��
//=============================================================================
void CGame::DeleteGameObject(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (GAME_DELETE_OBJECT)
			{// �Q�[���Ɏg�p���Ă���I�u�W�F�N�g�N���X������
				pScene->Uninit();
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck();
}

//=============================================================================
// �Q�[���̃X�e�[�W�ԍ��̌������v�Z���鏈��
//=============================================================================
void CGame::CalcStageDigits(void)
{
	m_nNumNumberDigits = (int)log10f((float)(m_nStageIdx + 1)) + 1;	// ���������߂�
	if (m_nNumNumberDigits <= 0) { m_nNumNumberDigits = 1; }	    // 0�ȉ��̂Ƃ�1�ɂ���
}



//*****************************************************************************
//
// �����p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̓ǂݍ��ރ}�b�v�t�@�C�����̃|�C���^�𐶐�����
//=============================================================================
void CGame::CreateMapFilePointer(void)
{
	m_pMapFileName = new char*[m_nNumMap];
	if (m_pMapFileName != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumMap; nCnt++)
		{
			m_pMapFileName[nCnt] = new char[256];
			strcpy(m_pMapFileName[nCnt], "\0");
		}
	}
}

//=============================================================================
// �Q�[���̃J������������
//=============================================================================
void CGame::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create();
	if (pCameraManager != NULL)
	{
		CGameCamera *pGameCamera = CGameCamera::Create();
		if (pGameCamera != NULL)
		{
			pCameraManager->SetCamera(pGameCamera);
		}
		SetCameraManager(pCameraManager);
	}
}

//=============================================================================
// �Q�[���̃e�N�X�`���Ǌ��N���X��������
//=============================================================================
void CGame::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}

//=============================================================================
// �Q�[���̃}�b�v��������
//=============================================================================
void CGame::CreateMap(void)
{
	CMap *pMap = CMap::Create(m_pMapFileName[m_nMapIdx]);
	SetMap(pMap);
	m_nNumEnemy = pMap->GetNumEnemyListData();
	ResetCounter();
	CEnemy::ResetSpawnCounter();
	SetPlayerPosToSpawn();
	CheckEnemySpawn(m_nGameCounter);
	ReleaseCreamMist();

	// UI��ύX
	if (m_pUI != NULL)
	{
		m_pUI->ReCreateEnemyIcon(m_nNumEnemy);
		m_pUI->ReCreateStageNumber((m_nStageIdx + 1));
	}

	// �A�C�e���̑��������Z�b�g
	CItem::ResetNumAll();
}

//=============================================================================
// �Q�[����UI��������
//=============================================================================
void CGame::CreateUI(void)
{
	m_pUI = CUI::Create(GetTextureManager(), 0, (m_nStageIdx + 1), &m_nPlayerStock[0], m_nNumberTexIdx);
}

//=============================================================================
// �Q�[���̃v���C���[��������
//=============================================================================
void CGame::CreatePlayer(void)
{
	// �v���C���[�𐶐�����
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = m_pPlayerManager[nCntPlayer]->SetPlayer(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nCntPlayer);
	}
}

//=============================================================================
// �Q�[���̓G��������
//=============================================================================
CEnemy *CGame::CreateEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CEnemy_ListData *pEnemyData)
{
	CEnemy *pEnemy = m_pEnemyManager[pEnemyData->GetEnemyType()]->SetEnemy(pos, rot, pEnemyData->GetEnemyType());

	if (pEnemy == NULL)return NULL;
	// �A�C�e���̃f�[�^��ݒ肷��
	if (pEnemyData->GetItem() == true)
	{
		pEnemy->SetItemData(pEnemyData->GetItemType());
	}

	// AI�̃f�[�^��ݒ肷��
	pEnemy->SetAIData(pEnemyData->GetAI_BulletEva(), pEnemyData->GetAI_MassEva(), pEnemyData->GetAI_DownEva());

	// �X�|�[���G�t�F�N�g�𐶐�����
	pEnemy->SetSpawnEffect();

	return pEnemy;
}

//=============================================================================
// �Q�[���̃A�C�e���𐶐����鏈��
//=============================================================================
CItem *CGame::CreateItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const int nItemType)
{
	CItem *pItem = NULL;
	int nModelIdx = 0;
	switch (nItemType)
	{
	case CItem::TYPE_STAR:
		nModelIdx = m_nItemModelIdx[0];
		pItem = CItemStar::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_GRENADE:
		nModelIdx = m_nItemModelIdx[1];
		pItem = CItemGrenade::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_1UP_TANK:
		nModelIdx = m_nItemModelIdx[2];
		pItem = CItem1up::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_SCOOP:
		nModelIdx = m_nItemModelIdx[3];
		pItem = CItemScoop::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_CLOCK:
		nModelIdx = m_nItemModelIdx[4];
		pItem = CItemClock::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_HELMET:
		nModelIdx = m_nItemModelIdx[5];
		pItem = CItemHelmet::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	}

	return pItem;
}

//=============================================================================
// �Q�[���̃Q�[���I�[�o�[���S�\���p�|���S���𐶐�����
//=============================================================================
void CGame::CreateGameOverLogo(void)
{
	m_pGameOverLogo = CScene2D::Create(m_GameOverPolyData.pos, m_GameOverPolyData.col,
		m_GameOverPolyData.fWidth, m_GameOverPolyData.fHeight, GAME_GAMEOVERLOGO_PRIORITY);
	if (m_pGameOverLogo != NULL, GetTextureManager() != NULL)
	{
		m_pGameOverLogo->BindTexture(GetTextureManager()->GetTexture(m_GameOverPolyData.nTexIdx));
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W�w�i�p�|���S���𐶐�����
//=============================================================================
void CGame::CreateStageBg(void)
{
	m_pStageBg = CScene2D::Create(m_StageBgData.pos, m_StageBgData.col, m_StageBgData.fWidth,
		m_StageBgData.fHeight, GAME_STAGEPOLYGON_PRIORITY);
}

//=============================================================================
// �Q�[���̃X�e�[�W���S�\���p�|���S���𐶐�����
//=============================================================================
void CGame::CreateStageLogo(void)
{
	m_pStageLogo = CScene2D::Create(m_StageLogoData.pos, m_StageLogoData.col,
		m_StageLogoData.fWidth, m_StageLogoData.fHeight, GAME_STAGEPOLYGON_PRIORITY);
	if (m_pStageLogo != NULL && GetTextureManager() != NULL)
	{
		m_pStageLogo->BindTexture(GetTextureManager()->GetTexture(m_StageLogoData.nTexIdx));
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W�ԍ��\���p�|���S���𐶐�����
//=============================================================================
void CGame::CreateStageNumber(void)
{
	int nStageNumber = 0;
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(m_nNumberTexIdx);
	}
	D3DXVECTOR3 NumberInterVal = D3DXVECTOR3(m_StageNumberData.fWidth + (m_StageNumberData.fWidth * GAME_NUMBER_INTERVAL_CUT), 0.0f, 0.0f);
	m_pStageNumber = CNumber::Create(m_StageNumberData.pos, m_StageNumberData.col, m_StageNumberData.fWidth,
		m_StageNumberData.fHeight, NumberInterVal, pTexture, (m_nStageIdx + 1), CNumberPolygon::STATE_NONE,
		3, 0.0f, GAME_STAGEPOLYGON_PRIORITY);
}

//=============================================================================
// �Q�[���̍��̉Ԃт�𐶐����鏈��
//=============================================================================
void CGame::CreateBlossoms(void)
{
	// �e��l�̌v�Z
	// �ړ���
	D3DXVECTOR3 Move = INITIALIZE_D3DXVECTOR3;
	Move.x = (float)((rand() % (m_CherryBlossomsData.nMoveXMax - m_CherryBlossomsData.nMoveXMin)) + m_CherryBlossomsData.nMoveXMin);
	Move.x *= -1.0f;
	Move.y = (float)((rand() % (m_CherryBlossomsData.nMoveYMax - m_CherryBlossomsData.nMoveYMin)) + m_CherryBlossomsData.nMoveYMin);

	// �p�x�̉�]�X�s�[�h
	float fAngleSpeed = (float)(rand() % (m_CherryBlossomsData.nAngleSpeedMax - m_CherryBlossomsData.nAngleSpeedMin) + m_CherryBlossomsData.nAngleSpeedMin);

	// �����̉�]�X�s�[�h
	float fRotSpeed = (float)(rand() % (m_CherryBlossomsData.nRotSpeedMax - m_CherryBlossomsData.nRotSpeedMin) + m_CherryBlossomsData.nRotSpeedMin);

	// ��
	float fWidth = (float)((rand() % (m_CherryBlossomsData.nWidthMax - m_CherryBlossomsData.nWidthMin)) + m_CherryBlossomsData.nWidthMin);

	// ����
	float fHeight = (float)((rand() % (m_CherryBlossomsData.nHeightMax - m_CherryBlossomsData.nHeightMin)) + m_CherryBlossomsData.nHeightMin);

	// �Ԃт�|���S������
	CBlossoms *pBlossoms = CBlossoms::Create(Move, D3DXToRadian(fAngleSpeed), D3DXToRadian(fRotSpeed), GAME_BLOSSOMS_POS_INI, GAME_BLOSSOMS_COL_INI, fWidth, fHeight);
	if (pBlossoms != NULL && GetTextureManager() != NULL)
	{
		pBlossoms->BindTexture(GetTextureManager()->GetTexture(m_CherryBlossomsData.nTexIdx));
	}
}

//=============================================================================
// �Q�[���̂ЂȂ����𐶐����鏈��
//=============================================================================
void CGame::CreateHinaarare(void)
{
	// �ЂȂ������o���ʒu���}�X�ڂ���v�Z
	D3DXVECTOR3 HinaararePos = INITIALIZE_D3DXVECTOR3;
	int nMassX = rand() % MASS_BLOCK_X;
	int nMassZ = rand() % MASS_BLOCK_Z;
	HinaararePos.x = (nMassX * MASS_SIZE_X) + MASS_SIZE_X_HALF - (MASS_BLOCK_X * MASS_SIZE_X_HALF);
	HinaararePos.z = (nMassZ * MASS_SIZE_Z) + MASS_SIZE_Z_HALF - (MASS_BLOCK_Z * MASS_SIZE_Z_HALF);
	HinaararePos.y = GAME_HINAARARE_POSY;

	// �ЂȂ����Ɏg�p���郂�f����ݒ�
	int nModelIdx = m_HinarareData.nModelIdx + (rand() % GAME_HINAARARE_MODEL_NUM);

	// �ЂȂ����𐶐�
	CModelCreate *pModelCreate = GetModelCreate();
	if (pModelCreate == NULL) { return; }
	CHinaarare::Create(HinaararePos, INITIALIZE_D3DXVECTOR3, pModelCreate->GetMesh(nModelIdx),
		pModelCreate->GetBuffMat(nModelIdx), pModelCreate->GetNumMat(nModelIdx),
		pModelCreate->GetTexture(nModelIdx));
}

//=============================================================================
// �Q�[���̃Q�[�������U���g�ɕK�v�Ȃ��̂𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult(void)
{
	// �|�����S�Ă̓G�̐���������
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_nNumAllBreakEnemy[nCntPlayer] = 0;
	}

	// �|�����S�Ă̓G�̐����v�Z
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
		{
			m_nNumAllBreakEnemy[nCntPlayer] += m_nNumBreakEnemy[nCntPlayer][nCntEnemy];
		}
	}

	// �{�[�i�X��t�^���Ă��Ȃ���Ԃ�
	m_bAddBonus = false;

	// �w�i���쐬
	CreateGameResult_Bg();

	// �X�e�[�W�ԍ��\���p�|���S�����쐬
	CreateGameResult_Stage();

	// �n�C�X�R�A�p�|���S�����쐬
	CreateGameResult_HighScore();

	// �v���C���[�X�R�A�p�|���S�����쐬
	CreateGameResult_PlayerScore();

	// �G�̃X�R�A�p���S�|���S�����쐬
	CreateGameResult_EnemyScore_Logo();

	// �g�[�^���X�R�A�p���S�|���S�����쐬
	CreateGameResult_TortalScore_Logo();

	// �i�s�x��߂��Ă���
	m_GameResultAddvance = GAMERESULT_NONE;
	m_nStateCounter = 0;
	m_bNextGameResult = true;
}

//=============================================================================
// �Q�[���̃Q�[�������U���g�p�w�i�𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_Bg(void)
{
	if (m_pGameResultBg == NULL)
	{
		m_pGameResultBg = CScene2D::Create(GAME_GAMERESULT_BG_POS_INI, GAME_GAMERESULT_BG_COL_INI,
			GAME_GAMERESULT_BG_WIDTH_INI, GAME_GAMERESULT_BG_HEIGHT_INI, GAME_GAMERESULT_BG_PRIORITY);
	}
}

//=============================================================================
// �Q�[���̃Q�[�������U���g�X�e�[�W�ԍ��\���p�|���S���𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_Stage(void)
{
	// ���S�|���S������
	if (m_pGameResultStage == NULL)
	{
		m_pGameResultStage = CScene2D::Create(GAME_GAMERESULT_STAGELOGO_POS_INI, GAME_GAMERESULT_STAGELOGO_COL_INI,
			GAME_GAMERESULT_STAGELOGO_WIDTH_INI, GAME_GAMERESULT_STAGELOGO_HEIGHT_INI, GAME_GAMERESULT_STAGELOGO_PRIORITY);
		if (m_pGameResultStage != NULL)
		{
			m_pGameResultStage->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_STAGELOGO_TEXIDX));
		}
	}

	// �ԍ��\���p�|���S������
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(m_nNumberTexIdx);
	}
	if (m_pGameResultStageNumber == NULL)
	{
		m_pGameResultStageNumber = CNumber::Create(GAME_GAMERESULT_STAGENUMBER_POS_INI, GAME_GAMERESULT_STAGENUMBER_COL_INI,
			GAME_GAMERESULT_STAGENUMBER_WIDTH_INI, GAME_GAMERESULT_STAGENUMBER_HEIGHT_INI, INITIALIZE_D3DXVECTOR3,
			pTexture, (m_nStageIdx + 1), CNumberPolygon::STATE_NONE, 3, 0.0f, GAME_GAMERESULT_STAGENUMBER_PRIORITY);
	}
}

//=============================================================================
// �Q�[���̃n�C�X�R�A�𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_HighScore(void)
{
	// ���S�|���S������
	if (m_pHighScoreLogo == NULL)
	{
		m_pHighScoreLogo = CScene2D::Create(GAME_GAMERESULT_HIGHSCORELOGO_POS_INI, GAME_GAMERESULT_HIGHSCORELOGO_COL_INI,
			GAME_GAMERESULT_HIGHSCORELOGO_WIDTH_INI, GAME_GAMERESULT_HIGHSCORELOGO_HEIGHT_INI, GAME_GAMERESULT_HIGHSCORELOGO_PRIORITY);
		if (m_pHighScoreLogo != NULL)
		{
			m_pHighScoreLogo->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_HIGHSCORELOGO_TEXIDX));
		}
	}

	// �ԍ��\���p�|���S������
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(m_nNumberTexIdx);
	}
	if (m_pHighScore == NULL)
	{
		m_pHighScore = CNumber::Create(GAME_GAMERESULT_HIGHSCORENUMBER_POS_INI, GAME_GAMERESULT_HIGHSCORENUMBER_COL_INI,
			GAME_GAMERESULT_HIGHSCORENUMBER_WIDTH_INI, GAME_GAMERESULT_HIGHSCORENUMBER_HEIGHT_INI, GAME_GAMERESULT_HIGHSCORENUMBER_INTERVAL_INI,
			pTexture, CTitle::GetHighScore(), CNumberPolygon::STATE_NONE, 3, 0.0f, GAME_GAMERESULT_HIGHSCORENUMBER_PRIORITY);
	}
}

//=============================================================================
// �Q�[���̃v���C���[�X�R�A�𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_PlayerScore(void)
{
	// �J��Ԃ��񐔂�ݒ�
	int nCountMax = MAX_NUM_PLAYER;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{
		nCountMax = MAX_NUM_PLAYER - 1;
	}

	// �|���S������
	D3DXVECTOR3 PlayerLogoPos = GAME_GAMERESULT_PLAYERLOGO_POS_INI;
	D3DXVECTOR3 PlayerNumberPos = GAME_GAMERESULT_PLAYERNUMBER_POS_INI;
	for (int nCnt = 0; nCnt < nCountMax; nCnt++)
	{
		// �v���C���[�̃X�R�A���v�Z
		m_nScore[nCnt] += m_nNumBreakEnemy[nCnt][CEnemy::TYPE_NORMAL] * ENEMY_SCORE_NORMAL;
		m_nScore[nCnt] += m_nNumBreakEnemy[nCnt][CEnemy::TYPE_ARMORE] * ENEMY_SCORE_ARMORE;
		m_nScore[nCnt] += m_nNumBreakEnemy[nCnt][CEnemy::TYPE_FAST] * ENEMY_SCORE_FAST;
		m_nScore[nCnt] += m_nNumBreakEnemy[nCnt][CEnemy::TYPE_HEAVY] * ENEMY_SCORE_HEAVY;

		// ���S�|���S������
		if (m_apPlayerLogo[nCnt] == NULL)
		{
			m_apPlayerLogo[nCnt] = CScene2D::Create(PlayerLogoPos, GAME_GAMERESULT_PLAYERLOGO_COL_INI,
				GAME_GAMERESULT_PLAYERLOGO_WIDTH_INI, GAME_GAMERESULT_PLAYERLOGO_HEIGHT_INI, GAME_GAMERESULT_PLAYERLOGO_PRIORITY);
			if (m_apPlayerLogo[nCnt] != NULL)
			{
				m_apPlayerLogo[nCnt]->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_PLAYERLOGO_TEXIDX + nCnt));
			}
		}

		// �ԍ��\���p�|���S������
		LPDIRECT3DTEXTURE9 pTexture = NULL;
		if (GetTextureManager() != NULL)
		{
			pTexture = GetTextureManager()->GetTexture(m_nNumberTexIdx);
		}
		if (m_apScore[nCnt] == NULL)
		{
			m_apScore[nCnt] = CNumber::Create(PlayerNumberPos, GAME_GAMERESULT_PLAYERNUMBER_COL_INI,
				GAME_GAMERESULT_PLAYERNUMBER_WIDTH_INI, GAME_GAMERESULT_PLAYERNUMBER_HEIGHT_INI, GAME_GAMERESULT_PLAYERNUMBER_INTERVAL_INI,
				pTexture, m_nScore[nCnt], CNumberPolygon::STATE_NONE, 3, 0.0f, GAME_GAMERESULT_PLAYERNUMBER_PRIORITY);
		}
		PlayerLogoPos.x = SCREEN_WIDTH / 2.0f + GAME_GAMERESULT_PLAYERLOGO_POS_INI.x;
		PlayerNumberPos.x = SCREEN_WIDTH / 2.0f + GAME_GAMERESULT_PLAYERNUMBER_POS_INI.x;
	}
}

//=============================================================================
// �Q�[���̓G�̃X�R�A�p���S�|���S���𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_EnemyScore_Logo(void)
{
	// �J��Ԃ��񐔂�ݒ�
	int nCountMax = MAX_NUM_PLAYER;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{
		nCountMax = MAX_NUM_PLAYER - 1;
	}

	// �|���S���𐶐�
	D3DXVECTOR3 EnemyArrowPos = GAME_GAMERESULT_ENEMYARROW_POS_INI;
	D3DXVECTOR3 EnemyScorePointLogoPos = GAME_GAMERESULT_ENEMYPOINTLOGO_POS_INI;
	for (int nCnt = 0; nCnt < nCountMax; nCnt++)
	{
		for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
		{// �G�̎�ނ̐������J��Ԃ�
			// ���|���S������
			if (m_apEnemyScoreArrow[nCnt][nCntEnemy] == NULL)
			{
				m_apEnemyScoreArrow[nCnt][nCntEnemy] = CScene2D::Create(EnemyArrowPos, GAME_GAMERESULT_ENEMYARROW_COL_INI,
					GAME_GAMERESULT_ENEMYARROW_WIDTH_INI, GAME_GAMERESULT_ENEMYARROW_HEIGHT_INI, GAME_GAMERESULT_ENEMYARROW_PRIORITY);
				if (m_apEnemyScoreArrow[nCnt][nCntEnemy] != NULL)
				{
					m_apEnemyScoreArrow[nCnt][nCntEnemy]->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_ENEMYARROW_TEXIDX + nCnt));
				}
			}
			EnemyArrowPos.y += GAME_GAMERESULT_ENEMYARROW_INTERVAL_INI.y;

			// �|�C���g�|���S������
			if (m_apEnemyScorePointLogo[nCnt][nCntEnemy] == NULL)
			{
				m_apEnemyScorePointLogo[nCnt][nCntEnemy] = CScene2D::Create(EnemyScorePointLogoPos, GAME_GAMERESULT_ENEMYPOINTLOGO_COL_INI,
					GAME_GAMERESULT_ENEMYPOINTLOGO_WIDTH_INI, GAME_GAMERESULT_ENEMYPOINTLOGO_HEIGHT_INI, GAME_GAMERESULT_ENEMYPOINTLOGO_PRIORITY);
				if (m_apEnemyScorePointLogo[nCnt][nCntEnemy] != NULL)
				{
					m_apEnemyScorePointLogo[nCnt][nCntEnemy]->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_ENEMYPOINTLOGO_TEXIDX));
				}
			}
			EnemyScorePointLogoPos.y += GAME_GAMERESULT_ENEMYPOINTLOGO_INTERVAL_INI.y;
		}
		EnemyArrowPos.x = (SCREEN_WIDTH / 2.0f + GAME_GAMERESULT_ENEMYARROW_INTERVAL_INI.x);
		EnemyArrowPos.y = GAME_GAMERESULT_ENEMYARROW_POS_INI.y;
		EnemyScorePointLogoPos.x = (SCREEN_WIDTH - GAME_GAMERESULT_ENEMYPOINTLOGO_INTERVAL_INI.x);
		EnemyScorePointLogoPos.y = GAME_GAMERESULT_ENEMYPOINTLOGO_POS_INI.y;
	}

	// �G�A�C�R���|���S���𐶐�
	D3DXVECTOR3 EnemyIconPos = GAME_GAMERESULT_ENEMYICON_POS_INI;
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{// �G�̎�ނ̐������J��Ԃ�
		if (m_apEnemyScoreIcon[nCntEnemy] == NULL)
		{
			m_apEnemyScoreIcon[nCntEnemy] = CScene2D::Create(EnemyIconPos, GAME_GAMERESULT_ENEMYICON_COL_INI,
				GAME_GAMERESULT_ENEMYICON_WIDTH_INI, GAME_GAMERESULT_ENEMYICON_HEIGHT_INI, GAME_GAMERESULT_ENEMYICON_PRIORITY);
			if (m_apEnemyScoreIcon[nCntEnemy] != NULL)
			{
				m_apEnemyScoreIcon[nCntEnemy]->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_ENEMYICON_TEXIDX + nCntEnemy));
			}
		}
		EnemyIconPos += GAME_GAMERESULT_ENEMYICON_INTERVAL_INI;
	}
}

//=============================================================================
// �Q�[���̓G�̃X�R�A�p�����|���S���𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_EnemyScore_Number(void)
{
	// �J��Ԃ��񐔂�ݒ�
	int nCountMax = MAX_NUM_PLAYER;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{
		nCountMax = MAX_NUM_PLAYER - 1;
	}

	int nEnemyScore[CEnemy::TYPE_MAX] = { ENEMY_SCORE_NORMAL, ENEMY_SCORE_ARMORE ,ENEMY_SCORE_FAST ,ENEMY_SCORE_HEAVY };
	D3DXVECTOR3 EnemyScorePos = GAME_GAMERESULT_ENEMYSCORE_POS_INI;
	D3DXVECTOR3 EnemyScoreInterval = D3DXVECTOR3(GAME_GAMERESULT_ENEMYSCORE_INTERVAL_INI.x, 0.0f, 0.0f);
	D3DXVECTOR3 EnemyBreakPos = GAME_GAMERESULT_ENEMYBREAK_POS_INI;
	D3DXVECTOR3 EnemyBreakInterval = D3DXVECTOR3(GAME_GAMERESULT_ENEMYBREAK_INTERVAL_INI.x, 0.0f, 0.0f);
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(m_nNumberTexIdx);
	}
	for (int nCnt = 0; nCnt < nCountMax; nCnt++)
	{
		// �X�R�A�|���S���𐶐�
		if (m_apEnemyScore[nCnt][m_GameResultAddvance] == NULL)
		{
			EnemyScorePos.y += m_GameResultAddvance * GAME_GAMERESULT_ENEMYSCORE_INTERVAL_INI.y;
			m_apEnemyScore[nCnt][m_GameResultAddvance] = CNumber::Create(EnemyScorePos,
				GAME_GAMERESULT_ENEMYSCORE_COL_INI,
				GAME_GAMERESULT_ENEMYSCORE_WIDTH_INI, GAME_GAMERESULT_ENEMYSCORE_HEIGHT_INI,
				EnemyScoreInterval, pTexture, 0,
				CNumberPolygon::STATE_NONE, 3, 0.0f, GAME_GAMERESULT_ENEMYSCORE_PRIORITY);
		}
		EnemyScorePos.x = SCREEN_WIDTH - GAME_GAMERESULT_ENEMYSCORE_POS_INI.x;
		EnemyScorePos.y = GAME_GAMERESULT_ENEMYSCORE_POS_INI.y;

		// �G��|�������|���S���𐶐�
		if (m_apNumBreakEnemy[nCnt][m_GameResultAddvance] == NULL)
		{
			EnemyBreakPos.y += m_GameResultAddvance * GAME_GAMERESULT_ENEMYBREAK_INTERVAL_INI.y;
			m_apNumBreakEnemy[nCnt][m_GameResultAddvance] = CNumber::Create(EnemyBreakPos,
				GAME_GAMERESULT_ENEMYBREAK_COL_INI,
				GAME_GAMERESULT_ENEMYBREAK_WIDTH_INI, GAME_GAMERESULT_ENEMYBREAK_HEIGHT_INI,
				EnemyBreakInterval, pTexture, 0,
				CNumberPolygon::STATE_NONE, 3, 0.0f, GAME_GAMERESULT_ENEMYBREAK_PRIORITY);
		}
		EnemyBreakPos.x = SCREEN_WIDTH - GAME_GAMERESULT_ENEMYBREAK_POS_INI.x - EnemyBreakInterval.x;
		EnemyBreakPos.y = GAME_GAMERESULT_ENEMYBREAK_POS_INI.y;
	}
}

//=============================================================================
// �Q�[���̃g�[�^���X�R�A�p���S�|���S���𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_TortalScore_Logo(void)
{
	// ���S�|���S������
	if (m_pTortalLogo == NULL)
	{
		m_pTortalLogo = CScene2D::Create(GAME_GAMERESULT_TOTALLOGO_POS_INI, GAME_GAMERESULT_TOTALLOGO_COL_INI,
			GAME_GAMERESULT_TOTALLOGO_WIDTH_INI, GAME_GAMERESULT_TOTALLOGO_HEIGHT_INI, GAME_GAMERESULT_TOTALLOGO_PRIORITY);
		if (m_pTortalLogo != NULL)
		{
			m_pTortalLogo->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_TOTALLOGO_TEXIDX));
		}
	}

	// ���C���|���S������
	if (m_pTortalLine == NULL)
	{
		m_pTortalLine = CScene2D::Create(GAME_GAMERESULT_TOTALLINE_POS_INI, GAME_GAMERESULT_TOTALLINE_COL_INI,
			GAME_GAMERESULT_TOTALLINE_WIDTH_INI, GAME_GAMERESULT_TOTALLINE_HEIGHT_INI, GAME_GAMERESULT_TOTALLINE_PRIORITY);
	}
}

//=============================================================================
// �Q�[���̃g�[�^���X�R�A�p�����|���S���𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_TortalScore_Number(void)
{
	// �J��Ԃ��񐔂�ݒ�
	int nCountMax = MAX_NUM_PLAYER;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{
		nCountMax = MAX_NUM_PLAYER - 1;
	}

	// �|���S���𐶐�
	D3DXVECTOR3 TortalScorePos = GAME_GAMERESULT_TOTALSCORE_POS_INI;
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(m_nNumberTexIdx);
	}
	for (int nCnt = 0; nCnt < nCountMax; nCnt++)
	{
		if (m_apNumAllBreakEnemy[nCnt] == NULL)
		{
			m_apNumAllBreakEnemy[nCnt] = CNumber::Create(TortalScorePos, GAME_GAMERESULT_TOTALSCORE_COL_INI,
				GAME_GAMERESULT_TOTALSCORE_WIDTH_INI, GAME_GAMERESULT_TOTALSCORE_HEIGHT_INI,
				GAME_GAMERESULT_TOTALSCORE_INTERVAL_INI, pTexture, m_nNumAllBreakEnemy[nCnt],
				CNumberPolygon::STATE_NONE, 3, 0.0f, GAME_GAMERESULT_TOTALSCORE_PRIORITY);
		}
		TortalScorePos.x = SCREEN_WIDTH - GAME_GAMERESULT_TOTALSCORE_POS_INI.x - GAME_GAMERESULT_TOTALSCORE_INTERVAL_INI.x;
	}
}

//=============================================================================
// �Q�[���̃{�[�i�X�X�R�A�𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_BonusScore(void)
{
	// �G��|�������������Ȃ�Ώ������Ȃ�
	if (m_nNumAllBreakEnemy[0] == m_nNumAllBreakEnemy[MAX_NUM_PLAYER - 1]) { return; }

	// �G��|���������������Ƀ{�[�i�X��t�^����
	D3DXVECTOR3 BonusScorePosAdd = INITIALIZE_D3DXVECTOR3;
	if (m_nNumAllBreakEnemy[0] < m_nNumAllBreakEnemy[MAX_NUM_PLAYER - 1])
	{
		m_nScore[MAX_NUM_PLAYER - 1] += GAME_BONUS_SCORE;
		BonusScorePosAdd.x = GAME_GAMERESULT_BONUS_ADDPOS;
	}
	else
	{
		m_nScore[0] += GAME_BONUS_SCORE;
	}

	// �\�����Ă���v���C���[�X�R�A��ϓ�
	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		if (m_apScore[nCnt] != NULL)
		{
			m_apScore[nCnt]->SetNumber(m_nScore[nCnt]);
		}
	}

	// ���S�|���S������
	if (m_pBonusScoreLogo == NULL)
	{
		m_pBonusScoreLogo = CScene2D::Create(GAME_GAMERESULT_BONUSSCORELOGO_POS_INI + BonusScorePosAdd, GAME_GAMERESULT_BONUSSCORELOGO_COL_INI,
			GAME_GAMERESULT_BONUSSCORELOGO_WIDTH_INI, GAME_GAMERESULT_BONUSSCORELOGO_HEIGHT_INI, GAME_GAMERESULT_BONUSSCORELOGO_PRIORITY);
		if (m_pBonusScoreLogo != NULL)
		{
			m_pBonusScoreLogo->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_BONUSSCORELOGO_TEXIDX));
		}
	}

	// �|�C���g�|���S������
	if (m_pBonusPointLogo == NULL)
	{
		m_pBonusPointLogo = CScene2D::Create(GAME_GAMERESULT_BONUSPOINTLOGO_POS_INI + BonusScorePosAdd, GAME_GAMERESULT_BONUSPOINTLOGO_COL_INI,
			GAME_GAMERESULT_BONUSPOINTLOGO_WIDTH_INI, GAME_GAMERESULT_BONUSPOINTLOGO_HEIGHT_INI, GAME_GAMERESULT_BONUSPOINTLOGO_PRIORITY);
		if (m_pBonusPointLogo != NULL)
		{
			m_pBonusPointLogo->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_BONUSPOINTLOGO_TEXIDX));
		}
	}

	// �����|���S���𐶐�
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(m_nNumberTexIdx);
	}
	if (m_pBonus == NULL)
	{
		m_pBonus = CNumber::Create(GAME_GAMERESULT_BONUSSCORE_POS_INI + BonusScorePosAdd, GAME_GAMERESULT_BONUSSCORE_COL_INI,
			GAME_GAMERESULT_BONUSSCORE_WIDTH_INI, GAME_GAMERESULT_BONUSSCORE_HEIGHT_INI,
			GAME_GAMERESULT_BONUSSCORE_INTERVAL_INI, pTexture, GAME_BONUS_SCORE,
			CNumberPolygon::STATE_NONE, 3, 0.0f, GAME_GAMERESULT_BONUSSCORE_PRIORITY);
	}

	// �{�[�i�X��t�^������Ԃ�
	m_bAddBonus = true;

	// ����炷
	CManager::GetSound()->PlaySound(GAME_SE_BONUS_IDX);
}

//=============================================================================
// �Q�[���̃|�[�Y�N���X�𐶐�����
//=============================================================================
void CGame::CreatePause(int nIdxPlayer)
{
	if (m_pPause != NULL) { return; }
	m_pPause = CPause::Create(GetTextureManager());

	// �|�[�Y�N���X�𐶐�
	if (m_pPause != NULL)
	{
		m_pPause->SetOpenId(nIdxPlayer);
	}

	// ���[�J��2�l�v���C�ȊO�Ȃ炱�̐揈�����Ȃ�
	if (CTitle::GetGameMode() != CTitle::GAMEMODE_LOCAL2P) { return; }

	// �ǂ��炪�|�[�Y���J�������\������p�|���S������
	if (m_pPausePlayerNumber == NULL)
	{
		m_pPausePlayerNumber = CScene2D::Create(GAME_PAUSE_PLAYERNUMBER_POS_INI, GAME_PAUSE_PLAYERNUMBER_COL_INI,
			GAME_PAUSE_PLAYERNUMBER_WIDTH_INI, GAME_PAUSE_PLAYERNUMBER_HEIGHT_INI, GAME_PAUSE_PLAYERNUMBER_PRIORITY);
		if (m_pPausePlayerNumber != NULL)
		{
			m_pPausePlayerNumber->BindTexture(GetTextureManager()->GetTexture(GAME_PAUSE_PLAYERNUMBER_TEXIDX + nIdxPlayer));
		}
	}
}

//=============================================================================
// �Q�[���̃|�[�Y��������ĂȂ����̃|���S���𐶐�����
//=============================================================================
void CGame::CreateNotPause(void)
{
	// ���w�i�𐶐�
	m_pNotPauseBlackBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
		GAME_PAUSE_NOT_BLACKBG_COL_INI, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, GAME_PAUSE_NOT_BLACKBG_PRIORITY);

	// ���S�|���S���𐶐�
	m_pNotPause = CScene2D::Create(GAME_PAUSE_NOT_POS_INI, GAME_PAUSE_NOT_COL_INI, GAME_PAUSE_NOT_WIDTH_INI,
		GAME_PAUSE_NOT_HEIGHT_INI, GAME_PAUSE_NOT_PRIORITY);
	if (m_pNotPause != NULL)
	{
		m_pNotPause->BindTexture(GetTextureManager()->GetTexture(GAME_PAUSE_NOT_TEXIDX));
	}
}

//=============================================================================
// �Q�[���̃C�x���g�J�n���S�𐶐�����
//=============================================================================
void CGame::CreateEventStartLogo(void)
{
	if (m_pEventStartLogo == NULL)
	{
		// �e�N�X�`��U���W�̓����_���Ōv�Z
		float fStartTexU = (rand() % GAME_MAPEVENT_STARTLOGO_TEXU_SPLIT) * (1.0f / GAME_MAPEVENT_STARTLOGO_TEXU_SPLIT);

		// �e�N�X�`��V���W�͎��̃C�x���g�ɂ���Đݒ�
		float fStartTexV = (1.0f / GAME_MAPEVENT_STARTLOGO_TEXV_SPLIT) * (m_nNextEvent - 1);

		m_pEventStartLogo = CScene2D::Create(GAME_MAPEVENT_STARTLOGO_POS, GAME_MAPEVENT_STARTLOGO_COL,
			GAME_MAPEVENT_STARTLOGO_WIDTH, GAME_MAPEVENT_STARTLOGO_HEIGHT, GAME_MAPEVENT_STARTLOGO_PRIORITY, 0.0f,
			fStartTexU, fStartTexV, (1.0f / GAME_MAPEVENT_STARTLOGO_TEXU_SPLIT),
			(1.0f / GAME_MAPEVENT_STARTLOGO_TEXV_SPLIT));
		if (m_pEventStartLogo != NULL)
		{
			m_pEventStartLogo->BindTexture(GetTextureManager()->GetTexture(GAME_MAPEVENT_STARTLOGO_TEXIDX));
		}
	}
}

//=============================================================================
// �Q�[���̃N���[���ɂ𐶐�����
//=============================================================================
void CGame::CreateMist(void)
{
	if (m_pCreamMist == NULL)
	{// ��������Ă��Ȃ����
		m_pCreamMist = CCreamMist::Create(GAME_CREAMMIST_POS_INI, GAME_CREAMMIST_COL_INI,
			GAME_CREAMMIST_WIDTH_INI, GAME_CREAMMIST_HEIGHT_INI, GAME_CREAMMIST_PRIORITY);
		if (m_pCreamMist != NULL)
		{
			m_pCreamMist->BindTexture(GetTextureManager()->GetTexture(GAME_CREAMMIST_TEXIDX));
		}
	}
	else
	{// ��������Ă����
		D3DXCOLOR col = m_pCreamMist->GetCol();
		col.a += GAME_CREAMMIST_ALPHA_UP;
		if (col.a >= GAME_CREAMMIST_ALPHA_MAX)
		{
			col.a = GAME_CREAMMIST_ALPHA_MAX;
		}
		m_pCreamMist->SetCol(col);
		m_pCreamMist->SetVtxBuffCol();
	}

	m_nMistCounter = 0;
}

//=============================================================================
// �Q�[���̐��͐}�𐶐�����
//=============================================================================
void CGame::CreatePowerMap(void)
{
	if (m_pPowerMap != NULL) { return; }
	m_pPowerMap = CPowerMap::Create(GetTextureManager());
}



//*****************************************************************************
//
// �J���p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̓ǂݍ��ރ}�b�v�t�@�C�����̃|�C���^���J������
//=============================================================================
void CGame::ReleaseMapFilePointer(void)
{
	if (m_pMapFileName != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumMap; nCnt++)
		{
			delete[] m_pMapFileName[nCnt];
			m_pMapFileName[nCnt] = NULL;
		}

		delete[] m_pMapFileName;
		m_pMapFileName = NULL;
	}
}

//=============================================================================
// �Q�[����UI�N���X���J������
//=============================================================================
void CGame::ReleaseUI(void)
{
	if (m_pUI != NULL)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = NULL;
	}
}

//=============================================================================
// �Q�[���̃v���C���[�N���X���J������
//=============================================================================
void CGame::ReleasePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer] != NULL)
		{
			m_pPlayer[nCntPlayer]->Uninit();
			m_pPlayer[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̃v���C���[�Ǌ��N���X���J������
//=============================================================================
void CGame::ReleasePlayerManager(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_pPlayerManager[nCntPlayer] != NULL)
		{
			m_pPlayerManager[nCntPlayer]->Uninit();
			delete m_pPlayerManager[nCntPlayer];
			m_pPlayerManager[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̓G�f�[�^�Ǌ��N���X���J������
//=============================================================================
void CGame::ReleaseEnemyManager(void)
{
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{
		if (m_pEnemyManager[nCntEnemy] != NULL)
		{
			m_pEnemyManager[nCntEnemy]->Uninit();
			delete m_pEnemyManager[nCntEnemy];
			m_pEnemyManager[nCntEnemy] = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̃Q�[���I�[�o�[���S�\���p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameOverLogo(void)
{
	if (m_pGameOverLogo != NULL)
	{
		m_pGameOverLogo->Uninit();
		m_pGameOverLogo = NULL;
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W�w�i�\���p�|���S�����J������
//=============================================================================
void CGame::ReleaseStageBg(void)
{
	if (m_pStageBg != NULL)
	{
		m_pStageBg->Uninit();
		m_pStageBg = NULL;
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W���S�\���p�|���S�����J������
//=============================================================================
void CGame::ReleaseStageLogo(void)
{
	if (m_pStageLogo != NULL)
	{
		m_pStageLogo->Uninit();
		m_pStageLogo = NULL;
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W�ԍ��\���p�|���S�����J������
//=============================================================================
void CGame::ReleaseStageNumber(void)
{
	if (m_pStageNumber != NULL)
	{
		m_pStageNumber->Uninit();
		m_pStageNumber = NULL;
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W�ԍ��\���p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult(void)
{
	ReleaseGameResult_Bg();
	ReleaseGameResult_Stage();
	ReleaseGameResult_HighScore();
	ReleaseGameResult_PlayerScore();
	ReleaseGameResult_EnemyScore();
	ReleaseGameResult_TortalScore();
	ReleaseGameResult_BonusScore();
}

//=============================================================================
// �Q�[���̃Q�[�����U���g�w�i�p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult_Bg(void)
{
	if (m_pGameResultBg != NULL)
	{
		m_pGameResultBg->Uninit();
		m_pGameResultBg = NULL;
	}
}

//=============================================================================
// �Q�[���̃Q�[�������U���g�X�e�[�W�\���p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult_Stage(void)
{
	// �X�e�[�W���S
	if (m_pGameResultStage != NULL)
	{
		m_pGameResultStage->Uninit();
		m_pGameResultStage = NULL;
	}

	// �X�e�[�W�ԍ��\���p�|���S��
	if (m_pGameResultStageNumber != NULL)
	{
		m_pGameResultStageNumber->Uninit();
		m_pGameResultStageNumber = NULL;
	}
}

//=============================================================================
// �Q�[���̃n�C�X�R�A�p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult_HighScore(void)
{
	// �����|���S��
	if (m_pHighScore != NULL)
	{
		if (m_pHighScore != NULL)
		{
			m_pHighScore->Uninit();
			m_pHighScore = NULL;
		}
	}

	// �n�C�X�R�A���S
	if (m_pHighScoreLogo != NULL)
	{
		m_pHighScoreLogo->Uninit();
		m_pHighScoreLogo = NULL;
	}
}

//=============================================================================
// �Q�[���̃v���C���[�X�R�A�p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult_PlayerScore(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{// �v���C���[�̑������J��Ԃ�
	    // �����|���S��
		if (m_apScore[nCntPlayer] != NULL)
		{
			m_apScore[nCntPlayer]->Uninit();
			m_apScore[nCntPlayer] = NULL;
		}

		// �v���C���[���S
		if (m_apPlayerLogo[nCntPlayer] != NULL)
		{
			m_apPlayerLogo[nCntPlayer]->Uninit();
			m_apPlayerLogo[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̃G�l�~�[�X�R�A�p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult_EnemyScore(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{// �v���C���[�̑������J��Ԃ�
		for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
		{// �G�̎�ޕ��J��Ԃ�
			// �G��|�������|���S��
			if (m_apNumBreakEnemy[nCntPlayer][nCntEnemy] != NULL)
			{
				m_apNumBreakEnemy[nCntPlayer][nCntEnemy]->Uninit();
				m_apNumBreakEnemy[nCntPlayer][nCntEnemy] = NULL;
			}

			// �G�̃X�R�A�\���p�|���S��
			if (m_apEnemyScore[nCntPlayer][nCntEnemy] != NULL)
			{
				m_apEnemyScore[nCntPlayer][nCntEnemy]->Uninit();
				m_apEnemyScore[nCntPlayer][nCntEnemy] = NULL;
			}

			// �|�C���g���S�\���p�|���S��
			if (m_apEnemyScorePointLogo[nCntPlayer][nCntEnemy] != NULL)
			{
				m_apEnemyScorePointLogo[nCntPlayer][nCntEnemy]->Uninit();
				m_apEnemyScorePointLogo[nCntPlayer][nCntEnemy] = NULL;
			}

			// �X�R�A���\���p�|���S��
			if (m_apEnemyScoreArrow[nCntPlayer][nCntEnemy] != NULL)
			{
				m_apEnemyScoreArrow[nCntPlayer][nCntEnemy]->Uninit();
				m_apEnemyScoreArrow[nCntPlayer][nCntEnemy] = NULL;
			}
		}
	}

	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{// �G�̎�ޕ��J��Ԃ�
		// �G�A�C�R���\���p�|���S��
		if (m_apEnemyScoreIcon[nCntEnemy] != NULL)
		{
			m_apEnemyScoreIcon[nCntEnemy]->Uninit();
			m_apEnemyScoreIcon[nCntEnemy] = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̃g�[�^���X�R�A�p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult_TortalScore(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{// �v���C���[�̑������J��Ԃ�
	    // �����|���S��
		if (m_apNumAllBreakEnemy[nCntPlayer] != NULL)
		{
			m_apNumAllBreakEnemy[nCntPlayer]->Uninit();
			m_apNumAllBreakEnemy[nCntPlayer] = NULL;
		}
	}

	// �g�[�^�����S�p�|���S��
	if (m_pTortalLogo != NULL)
	{
		m_pTortalLogo->Uninit();
		m_pTortalLogo = NULL;
	}

	// �g�[�^�����C���p�|���S��
	if (m_pTortalLine != NULL)
	{
		m_pTortalLine->Uninit();
		m_pTortalLine = NULL;
	}
}

//=============================================================================
// �Q�[���̃{�[�i�X�X�R�A�p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult_BonusScore(void)
{
	// �����|���S��
	if (m_pBonus != NULL)
	{
		m_pBonus->Uninit();
		m_pBonus = NULL;
	}

	// �{�[�i�X�X�R�A�|�C���g���S�\���p�|���S��
	if (m_pBonusPointLogo != NULL)
	{
		m_pBonusPointLogo->Uninit();
		m_pBonusPointLogo = NULL;
	}

	// �{�[�i�X�X�R�A���S�\���p�|���S��
	if (m_pBonusScoreLogo != NULL)
	{
		m_pBonusScoreLogo->Uninit();
		m_pBonusScoreLogo = NULL;
	}
}

//=============================================================================
// �Q�[���̃|�[�Y�N���X���J������
//=============================================================================
void CGame::ReleasePause(void)
{
	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = NULL;
	}

	if (m_pPausePlayerNumber != NULL)
	{
		m_pPausePlayerNumber->Uninit();
		m_pPausePlayerNumber = NULL;
	}
}

//=============================================================================
// �Q�[���̃|�[�Y��������ĂȂ����̃|���S�����J������
//=============================================================================
void CGame::ReleaseNotPause(void)
{
	// �����|���S���J��
	if (m_pNotPause != NULL)
	{
		m_pNotPause->Uninit();
		m_pNotPause = NULL;
	}

	// ���w�i�J��
	if (m_pNotPauseBlackBg != NULL)
	{
		m_pNotPauseBlackBg->Uninit();
		m_pNotPauseBlackBg = NULL;
	}
}


//=============================================================================
// �Q�[���̃|�[�Y��������ĂȂ����̃|���S�����J������
//=============================================================================
void CGame::ReleaseEventStartLogo(void)
{
	if (m_pEventStartLogo != NULL)
	{
		m_pEventStartLogo->Uninit();
		m_pEventStartLogo = NULL;
	}
}

//=============================================================================
// �Q�[���̃N���[���ɂ��J������
//=============================================================================
void CGame::ReleaseCreamMist(void)
{
	if (m_pCreamMist != NULL)
	{
		m_pCreamMist->Uninit();
		m_pCreamMist = NULL;
	}
}

//=============================================================================
// �Q�[���̐��͐}���J������
//=============================================================================
void CGame::ReleasePowerMap(void)
{
	if (m_pPowerMap != NULL)
	{
		m_pPowerMap->Uninit();
		m_pPowerMap = NULL;
	}
}



//*****************************************************************************
//
// �X�N���v�g�ǂݍ��ݗp�֐�(�}�b�v�C�x���g�p�ɃX�N���v�g�����Ȃ炱���ɁI�I)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̃V�X�e������ǂݍ���
//=============================================================================
void CGame::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(GAME_SYSTEM_FILENAME);
	if (pFileLoader != NULL)
	{
		strcpy(aStr, pFileLoader->GetString(aStr));
		if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
		{// �ǂݍ��݊J�n�̍��}������
			LoadSystemScript(pFileLoader, aStr);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̃V�X�e�������t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntMap = 0;
	int nCntTex = 0;
	int nCntModel = 0;
	int nCntItemModel = 0;
	int nCntEnemy = 0;
	int nCntPlayer = 0;

	while (1)
	{// ���[�v�J�n
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// �ǂݍ��ރe�N�X�`���̐�������
			CreateTexture(CFunctionLib::ReadInt(pStr, NUM_TEXTURE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// �ǂݍ��ރe�N�X�`���̃t�@�C����������
			LoadTexFileName(pStr, nCntTex);
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_MODEL) == 0)
		{// �ǂݍ��ރ��f���̐�������
			int nNumModel = CFunctionLib::ReadInt(pStr, NUM_MODEL);
			CModelCreate *pModelCreate = CModelCreate::Create(nNumModel);
			SetModelCreate(pModelCreate);
		}
		else if (CFunctionLib::Memcmp(pStr, MODEL_FILENAME) == 0)
		{// �ǂݍ��ރ��f���̃t�@�C����������
			LoadModel(pStr, nCntModel);
			nCntModel++;
		}
		else if (CFunctionLib::Memcmp(pStr, EFFECT_FILENAME) == 0)
		{// �ǂݍ��ރG�t�F�N�g�̃t�@�C����������
			LoadEffectFileName(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_MAP) == 0)
		{// �ǂݍ��ރ}�b�v�̐�������
			m_nNumMap = CFunctionLib::ReadInt(pStr, NUM_MAP);
			CreateMapFilePointer();
		}
		else if (CFunctionLib::Memcmp(pStr, MAP_FILENAME) == 0)
		{// �ǂݍ��ރ}�b�v�t�@�C����������
			LoadMapFileName(pStr, nCntMap);
			nCntMap++;
		}
		else if (CFunctionLib::Memcmp(pStr, ITEMEVENT_FILENAME) == 0)
		{// �A�C�e���C�x���g�p�X�N���v�g�t�@�C����������
			LoadItemEvent(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, NUMBER_TEX_IDX) == 0)
		{// �����̎g�p����e�N�X�`���ԍ�������
			m_nNumberTexIdx = CFunctionLib::ReadInt(pStr, NUMBER_TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, BULLET_MODEL_IDX) == 0)
		{// �e���g�p���郂�f���̔ԍ�������
			m_nBulletModelIdx = CFunctionLib::ReadInt(pStr, BULLET_MODEL_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, ITEM_MODEL_IDX) == 0)
		{// �A�C�e�����g�p���郂�f���̔ԍ�������
			m_nItemModelIdx[nCntItemModel] = CFunctionLib::ReadInt(pStr, ITEM_MODEL_IDX);
			nCntItemModel++;
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYERDATA_FILENAME) == 0)
		{// �v���C���[�f�[�^�̃X�N���v�g�t�@�C����������
			LoadPlayerFileName(pStr, nCntPlayer);
			nCntPlayer++;
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYER_STOCK_INI) == 0)
		{// �v���C���[�̎c�@�������l������
			m_nPlayerStockIni = CFunctionLib::ReadInt(pStr, PLAYER_STOCK_INI);
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYER_STOCK_MAX) == 0)
		{// �v���C���[�̎c�@���ő�l������
			m_nPlayerStockMax = CFunctionLib::ReadInt(pStr, PLAYER_STOCK_MAX);
		}
		else if (CFunctionLib::Memcmp(pStr, ENEMY_FILENAME) == 0)
		{// �G�f�[�^�̃X�N���v�g�t�@�C����������
			LoadEnemyFileName(pStr, nCntEnemy);
			nCntEnemy++;
		}
		else if (CFunctionLib::Memcmp(pStr, STAGEBGSET) == 0)
		{// �X�e�[�W�w�i�p�|���S����񂾂���
			LoadStageBg(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, STAGELOGOSET) == 0)
		{// �X�e�[�W���S�p�|���S����񂾂���
			LoadStageLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, STAGENUMBERSET) == 0)
		{// �X�e�[�W�ԍ��\���p�|���S����񂾂���
			LoadStageNumber(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, GAMEOVERPOLYGONSET) == 0)
		{// �Q�[���I�[�o�[���S�|���S����񂾂���
			LoadGameOverLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, HINAMATSURI_FILENAME) == 0)
		{// �ЂȍՂ�C�x���g�p�X�N���v�g�t�@�C����������
			LoadHinamatsuriEvent(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �Q�[���̓ǂݍ��ރe�N�X�`���t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadTexFileName(char *pStr, int nCntTex)
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	char aTexFileName[256] = "\0";
	strcpy(aTexFileName, CFunctionLib::ReadString(pStr, aTexFileName, TEXTURE_FILENAME));

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), aTexFileName, &pTexture);

	// �e�N�X�`���Ǌ��N���X�Ƀ|�C���^��ݒ肷��
	CTextureManager *pTextureManager = GetTextureManager();
	pTextureManager->SetTexture(pTexture, nCntTex);
	pTextureManager->SetFileName(pStr, nCntTex);
}

//=============================================================================
// �Q�[���̓ǂݍ��ރA�C�e�����f����ǂݍ���
//=============================================================================
void CGame::LoadModel(char *pStr, int nCntModel)
{
	// ���f���̃t�@�C���p�X����ǂݎ��
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat = 0;
	char aModelFileName[256] = "\0";
	strcpy(aModelFileName, CFunctionLib::ReadString(pStr, aModelFileName, MODEL_FILENAME));

	// x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(aModelFileName, D3DXMESH_SYSTEMMEM, CManager::GetRenderer()->GetDevice(), NULL,
		&pBuffMat, NULL, &nNumMat, &pMesh);

	// ���f���Ǌ��N���X�ɒl��ݒ肷��
	CModelCreate *pModelCreate = GetModelCreate();
	if (pModelCreate == NULL)return;
	pModelCreate->SetMesh(pMesh, nCntModel);
	pModelCreate->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);
	pModelCreate->SetFileName(pStr, nCntModel);
}

//=============================================================================
// �Q�[���̓ǂݍ��ރ}�b�v�t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadMapFileName(char *pStr, int nCntMap)
{
	strcpy(m_pMapFileName[nCntMap], CFunctionLib::ReadString(pStr, m_pMapFileName[nCntMap], MAP_FILENAME));
}

//=============================================================================
// �Q�[���̓ǂݍ��ރG�t�F�N�g�t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadEffectFileName(char *pStr)
{
	char aEffectFileName[256] = "\0";
	strcpy(aEffectFileName, CFunctionLib::ReadString(pStr, aEffectFileName, EFFECT_FILENAME));

	// �G�t�F�N�g�Ǌ��N���X�𐶐�
	CEffectManager *pEffectManager = CEffectManager::Create(aEffectFileName);
	SetEffectManager(pEffectManager);
}

//=============================================================================
// �Q�[���̓ǂݍ��ރv���C���[�f�[�^�t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadPlayerFileName(char *pStr, int nCntPlayer)
{
	char aPlayerFileName[256] = "\0";
	strcpy(aPlayerFileName, CFunctionLib::ReadString(pStr, aPlayerFileName, PLAYERDATA_FILENAME));

	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		if (nCntPlayer == CCharaSelect::GetPlayerNumber(nCnt))
		{
			m_pPlayerManager[nCnt] = CPlayerManager::Create(aPlayerFileName);
		}
	}
}

//=============================================================================
// �Q�[���̓ǂݍ��ޓG�f�[�^�t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadEnemyFileName(char *pStr, int nCntEnemy)
{
	char aEnemyFileName[256] = "\0";
	strcpy(aEnemyFileName, CFunctionLib::ReadString(pStr, aEnemyFileName, ENEMY_FILENAME));

	// �L�����N�^�[�Ǌ��N���X���쐬
	m_pEnemyManager[nCntEnemy] = CCharacterManager::Create(aEnemyFileName);
}

//=============================================================================
// �Q�[���̃X�e�[�W�w�i�|���S������ǂݍ���
//=============================================================================
void CGame::LoadStageBg(CFileLoader *pFileLoader, char *pStr)
{
	// �f�[�^������
	m_StageBgData.pos = GAME_STAGEBG_POS_INI;
	m_StageBgData.col = GAME_STAGEBG_COL_INI;
	m_StageBgData.fWidth = GAME_STAGEBG_WIDTH_INI;
	m_StageBgData.fHeight = GAME_STAGEBG_HEIGHT_INI;

	// �f�[�^�ǂݍ���
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			m_StageBgData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			m_StageBgData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_StageBgData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_StageBgData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_STAGEBGSET) == 0)
		{// �X�e�[�W�w�i�|���S�����I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W���S�|���S������ǂݍ���
//=============================================================================
void CGame::LoadStageLogo(CFileLoader *pFileLoader, char *pStr)
{
	// �f�[�^������
	m_StageLogoData.nTexIdx = 0;
	m_StageLogoData.pos = GAME_STAGELOGO_POS_INI;
	m_StageLogoData.col = GAME_STAGELOGO_COL_INI;
	m_StageLogoData.fWidth = GAME_STAGELOGO_WIDTH_INI;
	m_StageLogoData.fHeight = GAME_STAGELOGO_HEIGHT_INI;

	// �f�[�^�ǂݍ���
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			m_StageLogoData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			m_StageLogoData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			m_StageLogoData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_StageLogoData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_StageLogoData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_STAGELOGOSET) == 0)
		{// �X�e�[�W���S�|���S�����I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W�ԍ��\���p�|���S������ǂݍ���
//=============================================================================
void CGame::LoadStageNumber(CFileLoader *pFileLoader, char *pStr)
{
	// �f�[�^������
	m_StageNumberData.pos = GAME_STAGENUMBER_POS_INI;
	m_StageNumberData.col = GAME_STAGENUMBER_COL_INI;
	m_StageNumberData.fWidth = GAME_STAGENUMBER_WIDTH_INI;
	m_StageNumberData.fHeight = GAME_STAGENUMBER_HEIGHT_INI;

	// �f�[�^�ǂݍ���
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			m_StageNumberData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			m_StageNumberData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_StageNumberData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_StageNumberData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_STAGENUMBERSET) == 0)
		{// �X�e�[�W�ԍ��\���p�|���S�����I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �Q�[���̃Q�[���I�[�o�[���S�|���S������ǂݍ���
//=============================================================================
void CGame::LoadGameOverLogo(CFileLoader *pFileLoader, char *pStr)
{
	// �f�[�^������
	m_GameOverPolyData.nTexIdx = 0;
	m_GameOverPolyData.pos = GAME_GAMEOVERLOGO_POS_INI;
	m_GameOverPolyData.col = GAME_GAMEOVERLOGO_COL_INI;
	m_GameOverPolyData.fWidth = GAME_GAMEOVERLOGO_WIDTH_INI;
	m_GameOverPolyData.fHeight = GAME_GAMEOVERLOGO_HEIGHT_INI;

	// �f�[�^�ǂݍ���
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			m_GameOverPolyData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			m_GameOverPolyData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			m_GameOverPolyData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_GameOverPolyData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_GameOverPolyData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_GAMEOVERPOLYGONSET) == 0)
		{// �Q�[���I�[�o�[���S�|���S�����I���̍��}������
			break;
		}
	}
}


//*****************************************************************************
//
// �A�C�e���C�x���g�p�X�N���v�g�ǂݍ��ݏ���
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̃A�C�e���C�x���g�X�N���v�g��ǂݍ���
//=============================================================================
void CGame::LoadItemEvent(char *pStr)
{
	char aFileName[256] = "\0";
	strcpy(aFileName, CFunctionLib::ReadString(pStr, aFileName, ITEMEVENT_FILENAME));
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(aFileName);
	if (pFileLoader != NULL)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// �ǂݍ��݊J�n�̍��}������
			LoadItemEventScript(pFileLoader, pStr);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̃A�C�e���C�x���g�����t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadItemEventScript(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{// ���[�v�J�n
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SHAKESET) == 0)
		{// �J�����h��C�x���g���ǂݍ��݊J�n�̍��}������
			LoadItemEvent_Shake(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, ENEMY_STOP) == 0)
		{// �G���ǂꂭ�炢�~�߂邩��񂾂���
			m_ItemEventData.nStop = CFunctionLib::ReadInt(pStr, ENEMY_STOP);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �Q�[���̃J�����h��C�x���g�����t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadItemEvent_Shake(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{// ���[�v�J�n
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TIME) == 0)
		{// �h�炷���Ԃ�����
			m_ItemEventData.nShakeTime = CFunctionLib::ReadInt(pStr, TIME);
		}
		else if (CFunctionLib::Memcmp(pStr, VALUE) == 0)
		{// �ǂꂭ�炢�h�炷��������
			m_ItemEventData.fShake = CFunctionLib::ReadFloat(pStr, VALUE);
		}
		else if (CFunctionLib::Memcmp(pStr, CUT) == 0)
		{// �ǂꂭ�炢�h������炷��������
			m_ItemEventData.fShakeCut = CFunctionLib::ReadFloat(pStr, CUT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SHAKESET) == 0)
		{// �h��C�x���g�I���̍��}������
			break;
		}
	}
}


//*****************************************************************************
//
// �ЂȍՂ�C�x���g�p�X�N���v�g�ǂݍ��ݏ���
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̂ЂȍՂ�C�x���g�X�N���v�g��ǂݍ���
//=============================================================================
void CGame::LoadHinamatsuriEvent(char *pStr)
{
	char aFileName[256] = "\0";
	strcpy(aFileName, CFunctionLib::ReadString(pStr, aFileName, HINAMATSURI_FILENAME));
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(aFileName);
	if (pFileLoader != NULL)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// �ǂݍ��݊J�n�̍��}������
			LoadHinamatsuriEventScript(pFileLoader, pStr);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̂ЂȍՂ�C�x���g�����t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadHinamatsuriEventScript(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{// ���[�v�J�n
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, EVENT_RANDOM) == 0)
		{// �C�x���g�����_���]���l������
			m_nRandomEventEva = CFunctionLib::ReadInt(pStr, EVENT_RANDOM);
		}
		else if (CFunctionLib::Memcmp(pStr, CHERRYBLOSSOMSSET) == 0)
		{// ���̉Ԃт��񂾂���
			LoadCherryBlossomsData(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, HINAARARESET) == 0)
		{// �ЂȂ�����񂾂���
			LoadHinaarareData(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �Q�[���̍��̉Ԃт�f�[�^����ǂݍ���
//=============================================================================
void CGame::LoadCherryBlossomsData(CFileLoader *pFileLoader, char *pStr)
{
	// �f�[�^������
	m_CherryBlossomsData.nTime = GAME_BLOSSOMS_TIME_INI;
	m_CherryBlossomsData.nLap = GAME_BLOSSOMS_LAP_INI;
	m_CherryBlossomsData.nTexIdx = 0;
	m_CherryBlossomsData.nMoveXMax = GAME_BLOSSOMS_MOVEX_MAX_INI;
	m_CherryBlossomsData.nMoveXMin = GAME_BLOSSOMS_MOVEX_MIN_INI;
	m_CherryBlossomsData.nMoveYMax = GAME_BLOSSOMS_MOVEY_MAX_INI;
	m_CherryBlossomsData.nMoveYMin = GAME_BLOSSOMS_MOVEY_MIN_INI;
	m_CherryBlossomsData.nWidthMax = GAME_BLOSSOMS_WIDTH_MAX_INI;
	m_CherryBlossomsData.nWidthMin = GAME_BLOSSOMS_WIDTH_MIN_INI;
	m_CherryBlossomsData.nHeightMax = GAME_BLOSSOMS_HEIGHT_MAX_INI;
	m_CherryBlossomsData.nHeightMin = GAME_BLOSSOMS_HEIGHT_MIN_INI;
	m_CherryBlossomsData.nAngleSpeedMax = GAME_BLOSSOMS_ANGLESPEED_MAX_INI;
	m_CherryBlossomsData.nAngleSpeedMin = GAME_BLOSSOMS_ANGLESPEED_MIN_INI;
	m_CherryBlossomsData.nRotSpeedMax = GAME_BLOSSOMS_ROTSPEED_MAX_INI;
	m_CherryBlossomsData.nRotSpeedMin = GAME_BLOSSOMS_ROTSPEED_MIN_INI;

	// �f�[�^�ǂݍ���
	int nWord = 0;
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TIME) == 0)
		{// ���̉Ԃт炪�~�蒍������
			m_CherryBlossomsData.nTime = CFunctionLib::ReadInt(pStr, TIME);
		}
		else if (CFunctionLib::Memcmp(pStr, LAP) == 0)
		{// ���̉Ԃт��1�t���[�����ɂǂꂭ�炢�o����
			m_CherryBlossomsData.nLap = CFunctionLib::ReadInt(pStr, LAP);
		}
		else if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			m_CherryBlossomsData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, MOVEX) == 0)
		{// X�����̈ړ��ʂ�����
			m_CherryBlossomsData.nMoveXMax = CFunctionLib::ReadInt(pStr, MOVEX);
			pStr = CFunctionLib::HeadPutout(pStr, MOVEX);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nMoveXMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, MOVEY) == 0)
		{// Y�����̈ړ��ʂ�����
			m_CherryBlossomsData.nMoveYMax = CFunctionLib::ReadInt(pStr, MOVEY);
			pStr = CFunctionLib::HeadPutout(pStr, MOVEY);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nMoveYMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_CherryBlossomsData.nWidthMax = CFunctionLib::ReadInt(pStr, WIDTH);
			pStr = CFunctionLib::HeadPutout(pStr, WIDTH);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nWidthMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_CherryBlossomsData.nHeightMax = CFunctionLib::ReadInt(pStr, HEIGHT);
			pStr = CFunctionLib::HeadPutout(pStr, HEIGHT);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nHeightMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, ANGLESPEED) == 0)
		{// �p�x����]������X�s�[�h������
			m_CherryBlossomsData.nRotSpeedMax = CFunctionLib::ReadInt(pStr, ANGLESPEED);
			pStr = CFunctionLib::HeadPutout(pStr, ANGLESPEED);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nRotSpeedMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, ROTSPEED) == 0)
		{// ��������]������X�s�[�h������
			m_CherryBlossomsData.nRotSpeedMax = CFunctionLib::ReadInt(pStr, ROTSPEED);
			pStr = CFunctionLib::HeadPutout(pStr, ROTSPEED);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nRotSpeedMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, END_CHERRYBLOSSOMSSET) == 0)
		{// ���̉Ԃт�f�[�^�I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �Q�[���̂ЂȂ����f�[�^����ǂݍ���
//=============================================================================
void CGame::LoadHinaarareData(CFileLoader *pFileLoader, char *pStr)
{
	// �f�[�^������
	m_HinarareData.nModelIdx = GAME_HINAARARE_MODELIDX;
	m_HinarareData.nTime = GAME_HINAARARE_TIME;
	m_HinarareData.nAppear = GAME_HINAARARE_APPEAR;

	// �f�[�^�ǂݍ���
	int nWord = 0;
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, MODEL_IDX) == 0)
		{// �ЂȂ���ꂪ�g�p���郂�f���̔ԍ�
			m_HinarareData.nModelIdx = CFunctionLib::ReadInt(pStr, MODEL_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, TIME) == 0)
		{// �ЂȂ���ꂪ�~�蒍������
			m_HinarareData.nTime = CFunctionLib::ReadInt(pStr, TIME);
		}
		else if (CFunctionLib::Memcmp(pStr, APPEAR) == 0)
		{// �ЂȂ������o���Ԋu
			m_HinarareData.nAppear = CFunctionLib::ReadInt(pStr, APPEAR);
		}
		else if (CFunctionLib::Memcmp(pStr, END_HINAARARESET) == 0)
		{// �ЂȂ����f�[�^�I���̍��}������
			break;
		}
	}
}


//*****************************************************************************
//
// �ݒ�A�擾���F�X�֐�(�O���Ƃ̑������܂߂Ă����ɏ����Ă܂�)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̕ϐ�������������
//=============================================================================
void CGame::ClearVariable(void)
{
	m_State = STATE_STAGE_DISP;
	m_pUI = NULL;
	m_pMapFileName = NULL;
	m_nStateCounter = 0;
	m_nMapIdx = 0;
	m_nStageIdx = 0;
	m_nNumMap = 0;
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;
	m_nNumEnemy = 0;
	m_pGameOverLogo = NULL;
	m_pStageBg = NULL;
	m_pStageLogo = NULL;
	m_pStageNumber = NULL;
	m_nNumberTexIdx = 0;
	m_nNumNumberDigits = NULL;
	m_nPlayerRespawnCounter = 0;
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");
	m_bDeletePlayerFlag = 0;
	m_bHitBulletFlag = false;
	m_bEnemyMove = true;
	m_nEnemyMoveCounter = 0;
	m_HinaEvent = HINAEVENT_NORMAL;
	m_nEventCounter = 0;
	m_nNotEventCounter = 0;
	m_nRandomEventEva = 0;
	m_nPlayerStockIni = 0;
	m_nPlayerStockMax = 0;
	m_pEventStartLogo = NULL;
	m_nNextEvent = 0;

	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = NULL;
		m_pPlayerManager[nCntPlayer] = NULL;
		m_nPlayerStock[nCntPlayer] = 0;
	}
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{
		m_pEnemyManager[nCntEnemy] = NULL;
	}

	// �A�C�e���h���b�v�C�x���g
	m_pItem = NULL;
	m_pItemCylinder = NULL;
	m_ItemDropPos = INITIALIZE_D3DXVECTOR3;

	// �Q�[�������U���g�p�f�[�^
	m_GameResultAddvance = GAMERESULT_NONE;

	// �w�i�p
	m_pGameResultBg = NULL;

	// �X�e�[�W�ԍ��\���p
	m_pGameResultStage = NULL;
	m_pGameResultStageNumber = NULL;

	// �n�C�X�R�A�p
	m_pHighScore = NULL;
	m_pHighScoreLogo = NULL;

	// �v���C���[�X�R�A�p
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_nScore[nCntPlayer] = 0;
		m_apScore[nCntPlayer] = NULL;
		m_apPlayerLogo[nCntPlayer] = NULL;
	}

	// �G�l�~�[�X�R�A�p
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
		{
			m_nNumBreakEnemy[nCntPlayer][nCntEnemy] = 0;
			m_apNumBreakEnemy[nCntPlayer][nCntEnemy] = NULL;
			m_apEnemyScore[nCntPlayer][nCntEnemy] = NULL;
			m_apEnemyScorePointLogo[nCntPlayer][nCntEnemy] = NULL;
			m_apEnemyScoreArrow[nCntPlayer][nCntEnemy] = NULL;
		}
	}
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{
		m_apEnemyScoreIcon[nCntEnemy] = NULL;
	}

	// �g�[�^���X�R�A�p
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_nNumAllBreakEnemy[nCntPlayer] = 0;
		m_apNumAllBreakEnemy[nCntPlayer] = NULL;
		m_apPlayerLogo[nCntPlayer] = NULL;
	}
	m_pTortalLogo = NULL;
	m_pTortalLine = NULL;

	// �{�[�i�X�X�R�A�p
	m_pBonus = NULL;
	m_pBonusPointLogo = NULL;
	m_pBonusScoreLogo = NULL;

	// �|�[�Y�p
	m_bPauseOpen = false;
	m_pPause = NULL;
	m_pPausePlayerNumber = NULL;
	m_pNotPause = NULL;
	m_pNotPauseBlackBg = NULL;

	// �N���[���ɗp
	m_pCreamMist = NULL;
	m_nMistCounter = 0;

	// ���͐}�p
	m_pPowerMap = NULL;
}

//=============================================================================
// �Q�[���̒e���f����ݒ肷�鏈��
//=============================================================================
void CGame::SetBulletModel(CBullet *pBullet)
{
	pBullet->BindModel(GetModelCreate()->GetMesh(m_nBulletModelIdx), GetModelCreate()->GetBuffMat(m_nBulletModelIdx),
		GetModelCreate()->GetNumMat(m_nBulletModelIdx), GetModelCreate()->GetTexture(m_nBulletModelIdx));
}

//=============================================================================
// �Q�[���̏�Ԃ�ݒ肷��
//=============================================================================
void CGame::SetState(const STATE state)
{
	m_StateOld = m_State;
	m_State = state;
	m_nStateCounter = 0;
}

//=============================================================================
// �Q�[���̃v���C���[�N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CGame::SetPlayer(CPlayer *pPlayer, const int nIdx)
{
	m_pPlayer[nIdx] = pPlayer;
}

//=============================================================================
// �Q�[���̏o������G�̐���ݒ肷��
//=============================================================================
void CGame::SetNumEnemy(const int nNumEnemy)
{
	m_nNumEnemy = nNumEnemy;
}

//=============================================================================
// �W���}�[�̉��Z����
//=============================================================================
void CGame::AddJammer(int nIdx)
{
	m_nJammer[nIdx]++;
}

//=============================================================================
// �N���[���̉��Z����
//=============================================================================
void CGame::AddCream(int nIdx)
{
	m_nCream[nIdx]++;
}

//=============================================================================
// �R���N�^�[�̉��Z����
//=============================================================================
void CGame::AddCollecter(int nIdx)
{
	m_nCollector[nIdx]++;
}

//=============================================================================
// �Q�[���̏�Ԃ��擾����
//=============================================================================
CGame::STATE CGame::GetState(void)
{
	return m_State;
}

//=============================================================================
// �Q�[���̃v���C���[�N���X�ւ̃|�C���^���擾����
//=============================================================================
CPlayer *CGame::GetPlayer(const int nIdx)
{
	return m_pPlayer[nIdx];
}

//=============================================================================
// �Q�[���̏o������G�̐����擾����
//=============================================================================
int CGame::GetNumEnemy(void)
{
	return m_nNumEnemy;
}

//=============================================================================
// �Q�[���̓G�𓮂����邩�ǂ����擾����
//=============================================================================
bool CGame::GetEnemyMove(void)
{
	return m_bEnemyMove;
}

//=============================================================================
// �Q�[���̃X�e�[�W�ԍ����擾����
//=============================================================================
int CGame::GetStageIdx(void)
{
	return m_nStageIdx;
}