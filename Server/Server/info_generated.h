// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_CLIENTINFO_GAME_PROTOCOL_H_
#define FLATBUFFERS_GENERATED_CLIENTINFO_GAME_PROTOCOL_H_

#include "flatbuffers/flatbuffers.h"

namespace Game {
	namespace Protocol {

		struct Vec3;

		struct Client_Collection;

		struct Client_info;

		struct Monster_Collection;

		struct Monster_info;

		struct Item_Collection;

		struct Item_info;

		struct Init_Collection;

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

		struct Monster_Collection FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
			enum {
				VT_DATA = 4
			};
			const flatbuffers::Vector<flatbuffers::Offset<Monster_info>> *data() const {
				return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Monster_info>> *>(VT_DATA);
			}
			bool Verify(flatbuffers::Verifier &verifier) const {
				return VerifyTableStart(verifier) &&
					VerifyOffset(verifier, VT_DATA) &&
					verifier.VerifyVector(data()) &&
					verifier.VerifyVectorOfTables(data()) &&
					verifier.EndTable();
			}
		};

		struct Monster_CollectionBuilder {
			flatbuffers::FlatBufferBuilder &fbb_;
			flatbuffers::uoffset_t start_;
			void add_data(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Monster_info>>> data) {
				fbb_.AddOffset(Monster_Collection::VT_DATA, data);
			}
			explicit Monster_CollectionBuilder(flatbuffers::FlatBufferBuilder &_fbb)
				: fbb_(_fbb) {
				start_ = fbb_.StartTable();
			}
			Monster_CollectionBuilder &operator=(const Monster_CollectionBuilder &);
			flatbuffers::Offset<Monster_Collection> Finish() {
				const auto end = fbb_.EndTable(start_);
				auto o = flatbuffers::Offset<Monster_Collection>(end);
				return o;
			}
		};

		inline flatbuffers::Offset<Monster_Collection> CreateMonster_Collection(
			flatbuffers::FlatBufferBuilder &_fbb,
			flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Monster_info>>> data = 0) {
			Monster_CollectionBuilder builder_(_fbb);
			builder_.add_data(data);
			return builder_.Finish();
		}

		inline flatbuffers::Offset<Monster_Collection> CreateMonster_CollectionDirect(
			flatbuffers::FlatBufferBuilder &_fbb,
			const std::vector<flatbuffers::Offset<Monster_info>> *data = nullptr) {
			return Game::Protocol::CreateMonster_Collection(
				_fbb,
				data ? _fbb.CreateVector<flatbuffers::Offset<Monster_info>>(*data) : 0);
		}

		struct Monster_info FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
			enum {
				VT_ID = 4,
				VT_HP = 6,
				VT_ANIMATOR = 8,
				VT_DIRX = 10,
				VT_DIRZ = 12,
				VT_POSITION = 14,
				VT_ROTATION = 16
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
					VerifyField<Vec3>(verifier, VT_POSITION) &&
					VerifyField<Vec3>(verifier, VT_ROTATION) &&
					verifier.EndTable();
			}
		};

		struct Monster_infoBuilder {
			flatbuffers::FlatBufferBuilder &fbb_;
			flatbuffers::uoffset_t start_;
			void add_id(int32_t id) {
				fbb_.AddElement<int32_t>(Monster_info::VT_ID, id, 0);
			}
			void add_hp(int32_t hp) {
				fbb_.AddElement<int32_t>(Monster_info::VT_HP, hp, 0);
			}
			void add_animator(int32_t animator) {
				fbb_.AddElement<int32_t>(Monster_info::VT_ANIMATOR, animator, 0);
			}
			void add_dirX(float dirX) {
				fbb_.AddElement<float>(Monster_info::VT_DIRX, dirX, 0.0f);
			}
			void add_dirZ(float dirZ) {
				fbb_.AddElement<float>(Monster_info::VT_DIRZ, dirZ, 0.0f);
			}
			void add_position(const Vec3 *position) {
				fbb_.AddStruct(Monster_info::VT_POSITION, position);
			}
			void add_rotation(const Vec3 *rotation) {
				fbb_.AddStruct(Monster_info::VT_ROTATION, rotation);
			}
			explicit Monster_infoBuilder(flatbuffers::FlatBufferBuilder &_fbb)
				: fbb_(_fbb) {
				start_ = fbb_.StartTable();
			}
			Monster_infoBuilder &operator=(const Monster_infoBuilder &);
			flatbuffers::Offset<Monster_info> Finish() {
				const auto end = fbb_.EndTable(start_);
				auto o = flatbuffers::Offset<Monster_info>(end);
				return o;
			}
		};

		inline flatbuffers::Offset<Monster_info> CreateMonster_info(
			flatbuffers::FlatBufferBuilder &_fbb,
			int32_t id = 0,
			int32_t hp = 0,
			int32_t animator = 0,
			float dirX = 0.0f,
			float dirZ = 0.0f,
			const Vec3 *position = 0,
			const Vec3 *rotation = 0) {
			Monster_infoBuilder builder_(_fbb);
			builder_.add_rotation(rotation);
			builder_.add_position(position);
			builder_.add_dirZ(dirZ);
			builder_.add_dirX(dirX);
			builder_.add_animator(animator);
			builder_.add_hp(hp);
			builder_.add_id(id);
			return builder_.Finish();
		}

		struct Item_Collection FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
			enum {
				VT_DATA = 4
			};
			const flatbuffers::Vector<flatbuffers::Offset<Item_info>> *data() const {
				return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Item_info>> *>(VT_DATA);
			}
			bool Verify(flatbuffers::Verifier &verifier) const {
				return VerifyTableStart(verifier) &&
					VerifyOffset(verifier, VT_DATA) &&
					verifier.VerifyVector(data()) &&
					verifier.VerifyVectorOfTables(data()) &&
					verifier.EndTable();
			}
		};

		struct Item_CollectionBuilder {
			flatbuffers::FlatBufferBuilder &fbb_;
			flatbuffers::uoffset_t start_;
			void add_data(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Item_info>>> data) {
				fbb_.AddOffset(Item_Collection::VT_DATA, data);
			}
			explicit Item_CollectionBuilder(flatbuffers::FlatBufferBuilder &_fbb)
				: fbb_(_fbb) {
				start_ = fbb_.StartTable();
			}
			Item_CollectionBuilder &operator=(const Item_CollectionBuilder &);
			flatbuffers::Offset<Item_Collection> Finish() {
				const auto end = fbb_.EndTable(start_);
				auto o = flatbuffers::Offset<Item_Collection>(end);
				return o;
			}
		};

		inline flatbuffers::Offset<Item_Collection> CreateItem_Collection(
			flatbuffers::FlatBufferBuilder &_fbb,
			flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Item_info>>> data = 0) {
			Item_CollectionBuilder builder_(_fbb);
			builder_.add_data(data);
			return builder_.Finish();
		}

		inline flatbuffers::Offset<Item_Collection> CreateItem_CollectionDirect(
			flatbuffers::FlatBufferBuilder &_fbb,
			const std::vector<flatbuffers::Offset<Item_info>> *data = nullptr) {
			return Game::Protocol::CreateItem_Collection(
				_fbb,
				data ? _fbb.CreateVector<flatbuffers::Offset<Item_info>>(*data) : 0);
		}

		struct Item_info FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
			enum {
				VT_ID = 4,
				VT_TYPE = 6,
				VT_POSITION = 8
			};
			int32_t id() const {
				return GetField<int32_t>(VT_ID, 0);
			}
			int32_t type() const {
				return GetField<int32_t>(VT_TYPE, 0);
			}
			const Vec3 *position() const {
				return GetStruct<const Vec3 *>(VT_POSITION);
			}
			bool Verify(flatbuffers::Verifier &verifier) const {
				return VerifyTableStart(verifier) &&
					VerifyField<int32_t>(verifier, VT_ID) &&
					VerifyField<int32_t>(verifier, VT_TYPE) &&
					VerifyField<Vec3>(verifier, VT_POSITION) &&
					verifier.EndTable();
			}
		};

		struct Item_infoBuilder {
			flatbuffers::FlatBufferBuilder &fbb_;
			flatbuffers::uoffset_t start_;
			void add_id(int32_t id) {
				fbb_.AddElement<int32_t>(Item_info::VT_ID, id, 0);
			}
			void add_type(int32_t type) {
				fbb_.AddElement<int32_t>(Item_info::VT_TYPE, type, 0);
			}
			void add_position(const Vec3 *position) {
				fbb_.AddStruct(Item_info::VT_POSITION, position);
			}
			explicit Item_infoBuilder(flatbuffers::FlatBufferBuilder &_fbb)
				: fbb_(_fbb) {
				start_ = fbb_.StartTable();
			}
			Item_infoBuilder &operator=(const Item_infoBuilder &);
			flatbuffers::Offset<Item_info> Finish() {
				const auto end = fbb_.EndTable(start_);
				auto o = flatbuffers::Offset<Item_info>(end);
				return o;
			}
		};

		inline flatbuffers::Offset<Item_info> CreateItem_info(
			flatbuffers::FlatBufferBuilder &_fbb,
			int32_t id = 0,
			int32_t type = 0,
			const Vec3 *position = 0) {
			Item_infoBuilder builder_(_fbb);
			builder_.add_position(position);
			builder_.add_type(type);
			builder_.add_id(id);
			return builder_.Finish();
		}

		struct Init_Collection FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
			enum {
				VT_ID = 4,
				VT_ITEMDATA = 6,
				VT_MONSTERDATA = 8,
				VT_CLIENTDATA = 10
			};
			int32_t id() const {
				return GetField<int32_t>(VT_ID, 0);
			}
			const flatbuffers::Vector<flatbuffers::Offset<Item_info>> *itemData() const {
				return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Item_info>> *>(VT_ITEMDATA);
			}
			const flatbuffers::Vector<flatbuffers::Offset<Monster_info>> *MonsterData() const {
				return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Monster_info>> *>(VT_MONSTERDATA);
			}
			const flatbuffers::Vector<flatbuffers::Offset<Client_info>> *clientData() const {
				return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Client_info>> *>(VT_CLIENTDATA);
			}
			bool Verify(flatbuffers::Verifier &verifier) const {
				return VerifyTableStart(verifier) &&
					VerifyField<int32_t>(verifier, VT_ID) &&
					VerifyOffset(verifier, VT_ITEMDATA) &&
					verifier.VerifyVector(itemData()) &&
					verifier.VerifyVectorOfTables(itemData()) &&
					VerifyOffset(verifier, VT_MONSTERDATA) &&
					verifier.VerifyVector(MonsterData()) &&
					verifier.VerifyVectorOfTables(MonsterData()) &&
					VerifyOffset(verifier, VT_CLIENTDATA) &&
					verifier.VerifyVector(clientData()) &&
					verifier.VerifyVectorOfTables(clientData()) &&
					verifier.EndTable();
			}
		};

		struct Init_CollectionBuilder {
			flatbuffers::FlatBufferBuilder &fbb_;
			flatbuffers::uoffset_t start_;
			void add_id(int32_t id) {
				fbb_.AddElement<int32_t>(Init_Collection::VT_ID, id, 0);
			}
			void add_itemData(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Item_info>>> itemData) {
				fbb_.AddOffset(Init_Collection::VT_ITEMDATA, itemData);
			}
			void add_MonsterData(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Monster_info>>> MonsterData) {
				fbb_.AddOffset(Init_Collection::VT_MONSTERDATA, MonsterData);
			}
			void add_clientData(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Client_info>>> clientData) {
				fbb_.AddOffset(Init_Collection::VT_CLIENTDATA, clientData);
			}
			explicit Init_CollectionBuilder(flatbuffers::FlatBufferBuilder &_fbb)
				: fbb_(_fbb) {
				start_ = fbb_.StartTable();
			}
			Init_CollectionBuilder &operator=(const Init_CollectionBuilder &);
			flatbuffers::Offset<Init_Collection> Finish() {
				const auto end = fbb_.EndTable(start_);
				auto o = flatbuffers::Offset<Init_Collection>(end);
				return o;
			}
		};

		inline flatbuffers::Offset<Init_Collection> CreateInit_Collection(
			flatbuffers::FlatBufferBuilder &_fbb,
			int32_t id = 0,
			flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Item_info>>> itemData = 0,
			flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Monster_info>>> MonsterData = 0,
			flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Client_info>>> clientData = 0) {
			Init_CollectionBuilder builder_(_fbb);
			builder_.add_clientData(clientData);
			builder_.add_MonsterData(MonsterData);
			builder_.add_itemData(itemData);
			builder_.add_id(id);
			return builder_.Finish();
		}

		inline flatbuffers::Offset<Init_Collection> CreateInit_CollectionDirect(
			flatbuffers::FlatBufferBuilder &_fbb,
			int32_t id = 0,
			const std::vector<flatbuffers::Offset<Item_info>> *itemData = nullptr,
			const std::vector<flatbuffers::Offset<Monster_info>> *MonsterData = nullptr,
			const std::vector<flatbuffers::Offset<Client_info>> *clientData = nullptr) {
			return Game::Protocol::CreateInit_Collection(
				_fbb,
				id,
				itemData ? _fbb.CreateVector<flatbuffers::Offset<Item_info>>(*itemData) : 0,
				MonsterData ? _fbb.CreateVector<flatbuffers::Offset<Monster_info>>(*MonsterData) : 0,
				clientData ? _fbb.CreateVector<flatbuffers::Offset<Client_info>>(*clientData) : 0);
		}
		inline const Game::Protocol::Client_info *GetClientView(const void *buf) {
			return flatbuffers::GetRoot<Game::Protocol::Client_info>(buf);
		}
		inline const Game::Protocol::Monster_info *GetMonsterView(const void *buf) {
			return flatbuffers::GetRoot<Game::Protocol::Monster_info>(buf);
		}
		inline const Game::Protocol::Item_info *GetItemView(const void *buf) {
			return flatbuffers::GetRoot<Game::Protocol::Item_info>(buf);
		}

}  // namespace Protocol
}  // namespace Game

#endif  // FLATBUFFERS_GENERATED_CLIENTINFO_GAME_PROTOCOL_H_
