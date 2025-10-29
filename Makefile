# ============================
# SeminarGame Makefile (macOS, split build with stamp)
# - 初回のみ: make setup で raylib をビルド・キャッシュ
# - 以後は cpp の差分コンパイル＋リンクだけ
# - ディレクトリ重複ターゲットを避けるため stamp ファイル方式
# ============================

NAME      := game
CXX       := clang++
CC        := clang
STD       := c++17
OPT       := -O2
DEFS      := -DGRAPHICS_API_OPENGL_33
ARCH      ?= arm64   # arm64 / x86_64 / universal

# ---- アーキ設定 ----
ifeq ($(ARCH),universal)
  ARCHFLAGS    := -arch x86_64 -arch arm64
  RAY_ARCH_DIR := universal
else
  ARCHFLAGS    := -arch $(ARCH)
  RAY_ARCH_DIR := $(ARCH)
endif

# ---- 依存パス ----
RAYDIR    := external/raylib
RAYSRC    := $(RAYDIR)/src
RAYINC    := -I$(RAYDIR)/include -I$(RAYSRC) -I$(RAYSRC)/external/glfw/include

# raylib をキャッシュしておく場所
RAYCACHE  := build/raylib/$(RAY_ARCH_DIR)
RAYLIB    := $(RAYCACHE)/libraylib.a

# プロジェクト側ビルド
SRCS      := $(wildcard *.cpp)
BUILDDIR  := build/obj/$(RAY_ARCH_DIR)
STAMP     := $(BUILDDIR)/.stamp
OBJS      := $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SRCS))

CXXFLAGS  := -std=$(STD) $(OPT) $(DEFS) $(ARCHFLAGS) $(RAYINC)
LDFLAGS   := $(ARCHFLAGS)
FRAMEWORKS:= -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL \
             -framework Foundation -framework CoreAudio -framework AudioToolbox

# raylib を force_load で確実にリンク（静的リンク時の未参照落ち対策）
FORCELOAD := -Wl,-force_load,$(RAYLIB)

.PHONY: all run clean distclean setup archinfo _build_raylib_one _ensure_raylib

# -------------------------------
# ふだんの開発ループ: cpp だけ差分ビルド
# -------------------------------
all: _ensure_raylib $(NAME)

$(NAME): $(OBJS)
	@echo "==> Linking $(NAME) ($(ARCH))"
	$(CXX) $(LDFLAGS) $^ $(FORCELOAD) $(FRAMEWORKS) -o $@
	@echo "==> Build OK: ./$(NAME)"

# オブジェクト生成（順序専用依存にスタンプを使用）
$(BUILDDIR)/%.o: %.cpp | $(STAMP)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# スタンプ: ディレクトリ作成を一元化（重複ターゲット回避）
$(STAMP):
	@mkdir -p $(BUILDDIR)
	@touch $(STAMP)

run: all
	./$(NAME)

# -------------------------------
# 初回セットアップ: raylib をビルド＆キャッシュ
# -------------------------------
setup: $(RAYLIB)
	@echo "==> raylib is ready: $(RAYLIB)"

# universal は arm64/x86_64 を個別ビルドして lipo で合成
$(RAYLIB):
	@mkdir -p $(RAYCACHE)
ifeq ($(ARCH),universal)
	$(MAKE) _build_raylib_one ARCH=x86_64 OUT=$(RAYCACHE)/libraylib_x86_64.a
	$(MAKE) _build_raylib_one ARCH=arm64  OUT=$(RAYCACHE)/libraylib_arm64.a
	@lipo -create $(RAYCACHE)/libraylib_x86_64.a $(RAYCACHE)/libraylib_arm64.a -output $(RAYLIB)
	@echo "==> lipo: created universal $(RAYLIB)"
else
	$(MAKE) _build_raylib_one ARCH=$(ARCH) OUT=$(RAYLIB)
endif
	@echo "==> sanity-check symbols"
	@nm $(RAYLIB) | grep -E ' [Tt] _InitPlatform' >/dev/null || (echo "ERROR: _InitPlatform が見つかりません"; exit 1)

# 実際の raylib ビルド（1アーキテクチャ）
_build_raylib_one:
	@echo "==> Building raylib for $(ARCH)"
	@$(MAKE) -C $(RAYSRC) clean
	@$(MAKE) -C $(RAYSRC) PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=STATIC BUILD_MODE=RELEASE \
		CC="$(CC)" \
		CFLAGS="$(OPT) -arch $(ARCH) -DPLATFORM_DESKTOP -DPLATFORM_DESKTOP_GLFW -DGRAPHICS_API_OPENGL_33 -Wno-missing-braces -Werror=pointer-arith -fno-strict-aliasing -std=c99"
	@cp $(RAYSRC)/libraylib.a $(OUT)
	@echo "==> raylib build done: $(OUT)"

# raylib が無ければエラーで案内（setup を促す）
_ensure_raylib:
	@if [ ! -f "$(RAYLIB)" ]; then \
	  echo "ERROR: $(RAYLIB) が見つかりません。まず 'make setup ARCH=$(ARCH)' を実行してください。"; \
	  exit 1; \
	fi

# -------------------------------
# クリーン
# -------------------------------
clean:
	@echo "==> clean (keep raylib)"
	@rm -f $(NAME)
	@rm -rf $(BUILDDIR)
	@rm -f *.o *.d
	@rm -rf *.dSYM

distclean: clean
	@echo "==> distclean (also remove cached raylib)"
	@rm -rf build/raylib
	@$(MAKE) -C $(RAYSRC) clean || true

# -------------------------------
# 情報表示
# -------------------------------
archinfo:
	@echo "raylib:" ;  lipo -info $(RAYLIB) || echo "(not built)"
	@echo "binary:" ; lipo -info $(NAME)   || echo "(not built)"
