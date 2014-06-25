| relative | offset | absolute | size | description
| :------- | -----: | -------: | ---: | --------------
| ----     | 0000   | 0000     | 000c | `audio_container` chunk header [C]
| c        | 0000   | 000c     | 000c | `audio_header` chunk header [H]
| ch       | 0000   | 0018     | 0004 | size of track headers [T]
| ch       | 0020   | 0038     | 0004 | track count
| ch       | 0050   | 0068     | N*10 | first stream name
| chN      | -      | -        | T*34 | per-track data (see below)
| chT      | 0000   | -        | N*10 | second stream name
| cH       | 0000   | -        | 000c | `audio_data` chunk header
| cH       | 000c   | -        | -    | cluster sample data

Relative offsets are given as relative to the start (lower case) or end (upper
case) of the named chunk or segment, with segment names in []'s following their
description. For example, `chT` indicates an offset equal to the offset before
the beginning of the `audio_container` contents (skipping `0xc` bytes for the
chunk header), plus the similar offset for the `audio_header` chunk, plus the
named size "size of track headers". All numeric values above are given in
hexadecimal.

A chunk header looks like:

```
    uint32 chunk_type
    uint32 chunk_size
    uint32 chunk_version // 0x1c020065 for DotD
```

Relevant chunk types are:

```
    0x0000080d audio_container
    0x0000080e audio_header
    0x0000080f audio_data
```

The information following the first stream name (relative offset `ChN + 0x0`)
has the structure:

```
    [track_count] {
        uint32 unknown (identifier?)
        uint32 unknown (identifier?)
        uint32 unknown (identifier?)
        uint32 unknown (identifier?)
        uint32 unknown (zero)
        uint32 unknown (zero)
        uint32 track_byte_count
        uint32 track_byte_offset
    }
    [track_count] {
        uint32 track_total_used_bytes
    }
    [track_count] {
        uint32[4] unknown (checksum?)
    }
    [track_count] {
        char[8][] track_name
    }
    uint32 unknown (identifier?)
    uint32 unknown (identifier?)
    uint32 unknown (00 00 00 00)
    uint32 unknown (00 00 00 01)
    uint32 cluster_size
    uint32 unknown (identifier?)
    uint32 unknown (00 02 00 02)
    uint32 unknown
    uint32 cluster_used_bytes
    uint32 unknown
    uint32 sample_rate
    uint32 unknown
    uint32 unknown / first_track_sample_count?
    char unknown
    char num_channels
    uint16 unknown
    uint32[3] unknown
    uint32[4] unknown
    uint32 unused?
    uint32[4] unknown
```

(The notation `[count] { ... }` above indicates that the sub-structure in {}'s
repeats N times, where N is the value of `count`. Typical byte values are in
hexadecimal.)

The chunk headers are little endian. All other header data is big endian.
Samples are also big endian. Track offsets are measured from the start of the
`audio_data` chunk (following the chunk header for the same), i.e. an offset of
`0` refers to the first byte of the first data cluster. Track counts *include*
unused cluster bytes.

Samples are organized into clusters packed into the `audio_data` chunk with no
further headers or additional padding beyond the cluster padding. Each cluster
is `cluster_size` bytes long, of which only `cluster_used_bytes` contain sample
data (the remainder are unused padding).

Tracks are composed of samples stored in clusters. Each track's clusters are
contiguous, with the location and size described by `track_byte_offset` and
`track_byte_count`, where the offset is relative to the start of the
`audio_data` chunk data (i.e. with `0` being the first byte after the chunk
header of the `audio_data` chunk). A track is assembled by reading up to
`cluster_used_bytes` per cluster, up to `track_total_used_bytes`.
