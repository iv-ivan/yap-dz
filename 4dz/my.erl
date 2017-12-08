
-module(my).
-record(erl_image,
    {
      width,
      height
     }).

read_info(Fd) ->
    case file:read(Fd, 54) of
        {ok, <<_:18/bytes,Width:32/little,Height:32/little,_/bytes>> } ->
            {ok, #erl_image  {
                             width     = Width,
                             height    = Height
                            }}
    end.

change(Pixels, Text, CurPos, Size) ->
    case CurPos of
        Size ->
            Pixels;
        _ ->
            <<_:CurPos/bitstring,A:1/bitstring,_/bitstring>> = Text,
            Offset = CurPos*8+7,
            <<Z:Offset/bitstring,_:1/bitstring,X/bitstring>> = Pixels,
            NewPixels = <<Z/bitstring,A/bitstring,X/bitstring>>,
            NewCurPos = CurPos + 1,
            change(NewPixels, Text, NewCurPos, Size)
    end.

extract_size(Pixels, BitSize, Step) ->
    case Step of
        32 ->
            %erlang:display(BitSize),
            <<Size:32/integer>> = BitSize,
            {Size, Pixels};
        _ ->
            Offset = 7,
            <<_:Offset/bitstring,A:1/bitstring,NewPixels/bitstring>> = Pixels,
            NewBitSize = <<BitSize/bitstring,A/bitstring>>,
            NewStep = Step + 1,
            extract_size(NewPixels, NewBitSize, NewStep)
        end.

extract(Pixels, Text, CurPos, Size, SizeExtracted) ->
    case SizeExtracted of
        0 ->
            {ExtractedSize, CuttedPixels} = extract_size(Pixels, <<>>, 0),
            extract(CuttedPixels, Text, 0, ExtractedSize, 1);
        1 ->
            case CurPos of
                Size ->
                    Text;
                _ ->
                    Offset = CurPos*8+7,
                    <<_:Offset/bitstring,A:1/bitstring,_/bitstring>> = Pixels,
                    NewText = <<Text/bitstring,A/bitstring>>,
                    NewCurPos = CurPos + 1,
                    extract(Pixels, NewText, NewCurPos, Size, 1)
                end
        end.

find_good_byte(Text) ->
    PrefixSize = bit_size(Text) - 8,
    <<Prefix:PrefixSize/bitstring,Byte:8/bitstring>> = Text,
    <<First:1/bitstring,Second:1/bitstring,_/bitstring>> = Byte,
    case First of
        <<0:1>> ->
            Text;
        <<1:1>> ->
            case Second of
                <<1:1>> ->
                    Prefix;
                _ ->
                    find_good_byte(Prefix)
                end
        end.

fix_text(Text, Size) ->
    if 
        bit_size(Text) < Size ->
            Text;
        true ->
            SlicedSize = Size - Size rem 8,
            <<CutText:SlicedSize/bitstring,_/bitstring>> = Text,
            find_good_byte(CutText)
    end. 

main(_) ->
    {ok, File} = file:open("my.bmp",[read, binary]),
    {_, A} = read_info(File),
    Size = 3 * A#erl_image.height * A#erl_image.width,
    file:position(File, 54),
    {_, Pixels} = file:read(File, Size),
    file:close(File),
    {_, All} = file:read_file("my.bmp"),
    <<Header:54/bytes,_:Size/bytes,Other/bytes>> = All,
    {_, UTFText} = file:read_file("my.txt"),
    FixedText = fix_text(UTFText, Size - 32),
    LenText = bit_size(FixedText),
    LenTextBit = <<LenText:32/integer>>,
    Text = <<LenTextBit/bitstring, FixedText/bitstring>>,
    NewPixels = change(Pixels, Text, 0, min(Size, bit_size(Text))),
    NewBMP = <<Header/bytes,NewPixels/bytes,Other/bytes>>,
    file:write_file("my_coded.bmp", NewBMP),
    {_, CodedBMP} = file:read_file("my_coded.bmp"),
    <<_:54/bytes,CodedPixels:Size/bytes,_/bytes>> = CodedBMP,
    DecodedText = extract(CodedPixels, <<>>, 0, 0, 0),
    file:write_file("my_decoded.txt", DecodedText).