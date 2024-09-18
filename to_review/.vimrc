syntax on
set nu hls is ts=4 si sw=4
nnoremap <F8> :w<CR>:!make %< && echo "RUNNING" && ./%<<CR>
