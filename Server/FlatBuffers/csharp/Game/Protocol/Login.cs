// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace Game.Protocol
{

using global::System;
using global::FlatBuffers;

public struct Login : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static Login GetRootAsLogin(ByteBuffer _bb) { return GetRootAsLogin(_bb, new Login()); }
  public static Login GetRootAsLogin(ByteBuffer _bb, Login obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p.bb_pos = _i; __p.bb = _bb; }
  public Login __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public string Id { get { int o = __p.__offset(4); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetIdBytes() { return __p.__vector_as_span(4); }
#else
  public ArraySegment<byte>? GetIdBytes() { return __p.__vector_as_arraysegment(4); }
#endif
  public byte[] GetIdArray() { return __p.__vector_as_array<byte>(4); }
  public string Password { get { int o = __p.__offset(6); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetPasswordBytes() { return __p.__vector_as_span(6); }
#else
  public ArraySegment<byte>? GetPasswordBytes() { return __p.__vector_as_arraysegment(6); }
#endif
  public byte[] GetPasswordArray() { return __p.__vector_as_array<byte>(6); }

  public static Offset<Login> CreateLogin(FlatBufferBuilder builder,
      StringOffset idOffset = default(StringOffset),
      StringOffset passwordOffset = default(StringOffset)) {
    builder.StartObject(2);
    Login.AddPassword(builder, passwordOffset);
    Login.AddId(builder, idOffset);
    return Login.EndLogin(builder);
  }

  public static void StartLogin(FlatBufferBuilder builder) { builder.StartObject(2); }
  public static void AddId(FlatBufferBuilder builder, StringOffset idOffset) { builder.AddOffset(0, idOffset.Value, 0); }
  public static void AddPassword(FlatBufferBuilder builder, StringOffset passwordOffset) { builder.AddOffset(1, passwordOffset.Value, 0); }
  public static Offset<Login> EndLogin(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<Login>(o);
  }
};


}
