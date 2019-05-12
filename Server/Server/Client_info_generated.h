// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_CLIENTINFO_GAME_PROTOCOL_H_
#define FLATBUFFERS_GENERATED_CLIENTINFO_GAME_PROTOCOL_H_

#include "flatbuffers/flatbuffers.h"

namespace Game {
namespace Protocol {

struct Vec3;

struct Client_Collection;

struct Client_info;

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) Vec3 FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;
  float z_;

 public:
  Vec3() {
    memset(this, 0, sizeof(Vec3));
  }
  Vec3(float _x, float _y, float _z)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        z_(flatbuffers::EndianScalar(_z)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  float z() const {
    return flatbuffers::EndianScalar(z_);
  }
};
FLATBUFFERS_STRUCT_END(Vec3, 12);

struct Client_Collection FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_DATA = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<Client_info>> *data() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Client_info>> *>(VT_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.VerifyVector(data()) &&
           verifier.VerifyVectorOfTables(data()) &&
           verifier.EndTable();
  }
};

struct Client_CollectionBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_data(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Client_info>>> data) {
    fbb_.AddOffset(Client_Collection::VT_DATA, data);
  }
  explicit Client_CollectionBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  Client_CollectionBuilder &operator=(const Client_CollectionBuilder &);
  flatbuffers::Offset<Client_Collection> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Client_Collection>(end);
    return o;
  }
};

inline flatbuffers::Offset<Client_Collection> CreateClient_Collection(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Client_info>>> data = 0) {
  Client_CollectionBuilder builder_(_fbb);
  builder_.add_data(data);
  return builder_.Finish();
}

inline flatbuffers::Offset<Client_Collection> CreateClient_CollectionDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<Client_info>> *data = nullptr) {
  return Game::Protocol::CreateClient_Collection(
      _fbb,
      data ? _fbb.CreateVector<flatbuffers::Offset<Client_info>>(*data) : 0);
}

struct Client_info FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ID = 4,
    VT_HP = 6,
    VT_ANIMATOR = 8,
    VT_DIRX = 10,
    VT_DIRZ = 12,
    VT_HORIZONTAL = 14,
    VT_VERTICAL = 16,
    VT_NAME = 18,
    VT_POSITION = 20,
    VT_ROTATION = 22
  };
  int32_t id() const {
    return GetField<int32_t>(VT_ID, 0);
  }
  int32_t hp() const {
    return GetField<int32_t>(VT_HP, 0);
  }
  int32_t animator() const {
    return GetField<int32_t>(VT_ANIMATOR, 0);
  }
  float dirX() const {
    return GetField<float>(VT_DIRX, 0.0f);
  }
  float dirZ() const {
    return GetField<float>(VT_DIRZ, 0.0f);
  }
  float horizontal() const {
    return GetField<float>(VT_HORIZONTAL, 0.0f);
  }
  float vertical() const {
    return GetField<float>(VT_VERTICAL, 0.0f);
  }
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const Vec3 *position() const {
    return GetStruct<const Vec3 *>(VT_POSITION);
  }
  const Vec3 *rotation() const {
    return GetStruct<const Vec3 *>(VT_ROTATION);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           VerifyField<int32_t>(verifier, VT_HP) &&
           VerifyField<int32_t>(verifier, VT_ANIMATOR) &&
           VerifyField<float>(verifier, VT_DIRX) &&
           VerifyField<float>(verifier, VT_DIRZ) &&
           VerifyField<float>(verifier, VT_HORIZONTAL) &&
           VerifyField<float>(verifier, VT_VERTICAL) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<Vec3>(verifier, VT_POSITION) &&
           VerifyField<Vec3>(verifier, VT_ROTATION) &&
           verifier.EndTable();
  }
};

struct Client_infoBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int32_t id) {
    fbb_.AddElement<int32_t>(Client_info::VT_ID, id, 0);
  }
  void add_hp(int32_t hp) {
    fbb_.AddElement<int32_t>(Client_info::VT_HP, hp, 0);
  }
  void add_animator(int32_t animator) {
    fbb_.AddElement<int32_t>(Client_info::VT_ANIMATOR, animator, 0);
  }
  void add_dirX(float dirX) {
    fbb_.AddElement<float>(Client_info::VT_DIRX, dirX, 0.0f);
  }
  void add_dirZ(float dirZ) {
    fbb_.AddElement<float>(Client_info::VT_DIRZ, dirZ, 0.0f);
  }
  void add_horizontal(float horizontal) {
    fbb_.AddElement<float>(Client_info::VT_HORIZONTAL, horizontal, 0.0f);
  }
  void add_vertical(float vertical) {
    fbb_.AddElement<float>(Client_info::VT_VERTICAL, vertical, 0.0f);
  }
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Client_info::VT_NAME, name);
  }
  void add_position(const Vec3 *position) {
    fbb_.AddStruct(Client_info::VT_POSITION, position);
  }
  void add_rotation(const Vec3 *rotation) {
    fbb_.AddStruct(Client_info::VT_ROTATION, rotation);
  }
  explicit Client_infoBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  Client_infoBuilder &operator=(const Client_infoBuilder &);
  flatbuffers::Offset<Client_info> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Client_info>(end);
    return o;
  }
};

inline flatbuffers::Offset<Client_info> CreateClient_info(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t id = 0,
    int32_t hp = 0,
    int32_t animator = 0,
    float dirX = 0.0f,
    float dirZ = 0.0f,
    float horizontal = 0.0f,
    float vertical = 0.0f,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    const Vec3 *position = 0,
    const Vec3 *rotation = 0) {
  Client_infoBuilder builder_(_fbb);
  builder_.add_rotation(rotation);
  builder_.add_position(position);
  builder_.add_name(name);
  builder_.add_vertical(vertical);
  builder_.add_horizontal(horizontal);
  builder_.add_dirZ(dirZ);
  builder_.add_dirX(dirX);
  builder_.add_animator(animator);
  builder_.add_hp(hp);
  builder_.add_id(id);
  return builder_.Finish();
}

inline flatbuffers::Offset<Client_info> CreateClient_infoDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t id = 0,
    int32_t hp = 0,
    int32_t animator = 0,
    float dirX = 0.0f,
    float dirZ = 0.0f,
    float horizontal = 0.0f,
    float vertical = 0.0f,
    const char *name = nullptr,
    const Vec3 *position = 0,
    const Vec3 *rotation = 0) {
  return Game::Protocol::CreateClient_info(
      _fbb,
      id,
      hp,
      animator,
      dirX,
      dirZ,
      horizontal,
      vertical,
      name ? _fbb.CreateString(name) : 0,
      position,
      rotation);
}
inline const Game::Protocol::Client_info *GetClientView(const void *buf) {
	return flatbuffers::GetRoot<Game::Protocol::Client_info>(buf);
}
}  // namespace Protocol
}  // namespace Game

#endif  // FLATBUFFERS_GENERATED_CLIENTINFO_GAME_PROTOCOL_H_
